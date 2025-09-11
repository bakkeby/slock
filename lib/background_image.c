#include <X11/extensions/XShm.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#if HAVE_IMLIB
#include <Imlib2.h>
#endif

XShmSegmentInfo shminfo;
XImage *image = NULL;

char *
expandhome(const char *string)
{
	int path_length;
	char *ret;

	/* Substitute ~ with home directory */
	if (startswith("~/", string)) {
		const char *home = getenv("HOME");
		if (startswith("/", home)) {
			path_length = strlen(string) + strlen(home);
			ret = calloc(path_length, sizeof(char));
			snprintf(ret, path_length, "%s%s", home, string+1);
			return ret;
		}
	}

	return strdup(string);
}

void
create_lock_image(Display *dpy, struct lock *lock)
{
	int i;

	/* Create an XImage that is in shared memory between client <-> X server */
	image = XShmCreateImage(
		dpy,
		DefaultVisual(dpy, lock->screen),
		DefaultDepth(dpy, lock->screen),
		ZPixmap,
		NULL,
		&shminfo,
		lock->x,
		lock->y
	);

	shminfo.shmid = shmget(IPC_PRIVATE, image->bytes_per_line * image->height, IPC_CREAT | 0777);
	shminfo.shmaddr = image->data = shmat(shminfo.shmid, 0, 0);
	shminfo.readOnly = False;

	XShmAttach(dpy, &shminfo);

	/* Background pixmap */
	lock->bgmap = XCreatePixmap(dpy, lock->win, lock->x, lock->y, DefaultDepth(dpy, lock->screen));

	if (enabled(BackgroundImage) && !load_image_from_file(dpy, lock, background_image)) {
		/* Fall back to grabbing a screenshot into a client-side XImage
		 * and copy the data to our shared memory image */
		XImage *rootimg = XGetImage(dpy, lock->root, 0, 0, lock->x, lock->y, AllPlanes, ZPixmap);
		memcpy(image->data, rootimg->data,
		       image->bytes_per_line * image->height);
		XDestroyImage(rootimg);
	}

	/* Apply background filters (if any) */
	for (i = 0; i < num_background_filters; i++) {
		if (background_filters[i].func) {
			background_filters[i].func(image, background_filters[i].parameters, lock);
		}
	}

	/* Push image into pixmap and set window background */
	XShmPutImage(dpy, lock->bgmap, lock->gc, image, 0, 0, 0, 0, lock->x, lock->y, False);
	XSetWindowBackgroundPixmap(dpy, lock->win, lock->bgmap);
	XClearWindow(dpy, lock->win);
	XFlush(dpy);
}

void
cleanup_lock_image(Display *dpy)
{
	XShmDetach(dpy, &shminfo);
	shmdt(shminfo.shmaddr);
	shmctl(shminfo.shmid, IPC_RMID, 0);
	XDestroyImage(image);
}

int
load_image_from_file(Display *dpy, struct lock *lock, const char *filename)
{
	if (!filename || !strlen(filename))
		return 0;

	#if HAVE_IMLIB
	Imlib_Image img = imlib_load_image(filename);
	if (!img) {
		fprintf(stderr, "error: failed to load image %s with imlib2\n", filename);
		return 0;
	}

	imlib_context_set_image(img);

	int width = imlib_image_get_width();
	int height = imlib_image_get_height();

	/* Create a scaled copy if needed */
	Imlib_Image scaled = imlib_create_cropped_scaled_image(
		0, 0, width, height,
		lock->mw, lock->mh
	);

	/* Free original if scaled image was created */
	if (scaled) {
		imlib_free_image();
		imlib_context_set_image(scaled);
		width = lock->mw;
		height = lock->mh;
	} else {
		/* Scaling failed, just use original */
		scaled = img;
	}

	uint32_t *data = imlib_image_get_data();
	if (!data) {
		fprintf(stderr, "error: imlib2 returned NULL data for %s\n", filename);
		imlib_free_image();
		return 0;
	}

	/* Copy imlib2's 32-bit ARGB data into our XImage */
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			uint32_t argb = data[y * width + x];

			/* Imlib2 gives pixels as 0xAARRGGBB (MSB first) */
			// uint8_t a = (argb >> 24) & 0xFF;
			uint8_t r = (argb >> 16) & 0xFF;
			uint8_t g = (argb >> 8)  & 0xFF;
			uint8_t b = (argb)       & 0xFF;

			/* Ignore alpha (or you can blend if you want transparency) */
			uint32_t pixel = (r << 16) | (g << 8) | b;

			if (lock->m->mx + x < image->width && lock->m->my + y < image->height) {
				XPutPixel(image, lock->m->mx + x, lock->m->my + y, pixel);
			}
		}
	}

	imlib_free_image();

	#else // No IMLIB, fall back to loading farbfeld image

	int fd = open(filename, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "error: cannot open %s: %s\n", filename, strerror(errno));
		return 0;
	}
	uint8_t header[16];
	ssize_t n = read(fd, header, sizeof(header));
	if (n != sizeof(header)) {
		fprintf(stderr, "error: failed to read farbfeld header from %s\n", filename);
		close(fd);
		return -1;
	}

	if (memcmp(header, "farbfeld", 8) != 0) {
		fprintf(stderr, "error: %s is not a farbfeld file\n", filename);
		close(fd);
		return -1;
	}

	uint32_t width = (header[8] << 24) | (header[9] << 16) | (header[10] << 8) | header[11];
	uint32_t height = (header[12] << 24) | (header[13] << 16) | (header[14] << 8) | header[15];

	size_t pixels = width * height;
	uint8_t *ffbuf = malloc(pixels * 8);
	read(fd, ffbuf, pixels * 8);
	close(fd);

	/* Fill into image->data */
	for (uint32_t y = 0; y < height; y++) {
		for (uint32_t x = 0; x < width; x++) {
			uint8_t *src = ffbuf + (y * width + x) * 8;
			uint16_t r16 = (src[0] << 8) | src[1];
			uint16_t g16 = (src[2] << 8) | src[3];
			uint16_t b16 = (src[4] << 8) | src[5];
			// uint16_t a16 = (src[6] << 8) | src[7];  /* alpha ignored unless you do blending */

			uint8_t r8 = r16 >> 8;
			uint8_t g8 = g16 >> 8;
			uint8_t b8 = b16 >> 8;
			// uint8_t a8 = a16 >> 8;  /* alpha ignored unless you do blending */

			/* Write into image depending on byte order/format */
			uint32_t pixel = (r8 << 16) | (g8 << 8) | (b8);
			XPutPixel(image, x + lock->m->mx, y + lock->m->my, pixel);
		}
	}
	#endif

	return 1;
}
