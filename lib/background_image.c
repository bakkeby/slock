#include <Imlib2.h>

Imlib_Image image;


int startswith(const char *needle, const char *haystack)
{
	return !strncmp(haystack, needle, strlen(needle));
}

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
render_lock_image(Display *dpy, struct lock *lock, Imlib_Image image)
{
	if (image) {
		lock->bgmap = XCreatePixmap(dpy, lock->root, DisplayWidth(dpy, lock->screen), DisplayHeight(dpy, lock->screen), DefaultDepth(dpy, lock->screen));
		if (!background_image || !strlen(background_image)) {
			imlib_context_set_image(image);
		}
		imlib_context_set_display(dpy);
		imlib_context_set_visual(DefaultVisual(dpy, lock->screen));
		imlib_context_set_colormap(DefaultColormap(dpy, lock->screen));
		imlib_context_set_drawable(lock->bgmap);
		imlib_render_image_on_drawable(0, 0);
		imlib_free_image();
	}
}

void
create_lock_image(Display *dpy)
{
	if (background_image && strlen(background_image)) {
		if (load_lock_image_from_file(dpy, background_image))
			return;
	}

	/* Create screenshot Image */
	Screen *scr = ScreenOfDisplay(dpy, DefaultScreen(dpy));
	image = imlib_create_image(scr->width,scr->height);
	imlib_context_set_image(image);
	imlib_context_set_display(dpy);
	imlib_context_set_visual(DefaultVisual(dpy,0));
	imlib_context_set_drawable(RootWindow(dpy,XScreenNumberOfScreen(scr)));
	imlib_copy_drawable_to_image(0,0,0,scr->width,scr->height,0,0,1);

	/* Pixelation */
	if (background_pixel_size) {
		int width = scr->width;
		int height = scr->height;
		int pixel_size = background_pixel_size;

		for (int y = 0; y < height; y += pixel_size) {
			for (int x = 0; x < width; x += pixel_size) {
				int red = 0;
				int green = 0;
				int blue = 0;

				Imlib_Color pixel;
				Imlib_Color* pp;
				pp = &pixel;
				for (int j = 0; j < pixel_size && j < height; j++) {
					for (int i = 0; i < pixel_size && i < width; i++) {
						imlib_image_query_pixel(x + i, y + j, pp);
						red += pixel.red;
						green += pixel.green;
						blue += pixel.blue;
					}
				}
				red /= (pixel_size * pixel_size);
				green /= (pixel_size * pixel_size);
				blue /= (pixel_size * pixel_size);
				imlib_context_set_color(red, green, blue, pixel.alpha);
				imlib_image_fill_rectangle(x, y, pixel_size, pixel_size);
				red = 0;
				green = 0;
				blue = 0;
			}
		}
	} else {
		/* Blur function */
		imlib_image_blur(background_blur_radius);
	}
}

int
load_lock_image_from_file(Display *dpy, const char *background_image)
{
	/* Load picture */
	char *expanded_path = expandhome(background_image);
	Imlib_Image buffer = imlib_load_image(expanded_path);
	free(expanded_path);

	if (!buffer) {
		fprintf(stderr, "Failed to load background image: %s\n", background_image);
		return 0;
	}

	imlib_context_set_image(buffer);
	int background_image_width = imlib_image_get_width();
	int background_image_height = imlib_image_get_height();

	/* Create an image to be rendered */
	Screen *scr = ScreenOfDisplay(dpy, DefaultScreen(dpy));
	image = imlib_create_image(scr->width, scr->height);
	imlib_context_set_image(image);

	/* Fill the image for every X monitor */
	XRRMonitorInfo	*monitors;
	int number_of_monitors;
	monitors = XRRGetMonitors(dpy, RootWindow(dpy, XScreenNumberOfScreen(scr)), True, &number_of_monitors);

	int i;
	for (i = 0; i < number_of_monitors; i++) {
	    imlib_blend_image_onto_image(buffer, 0, 0, 0, background_image_width, background_image_height, monitors[i].x, monitors[i].y, monitors[i].width, monitors[i].height);
	}

	/* Clean up */
	imlib_context_set_image(buffer);
	imlib_free_image();
	imlib_context_set_image(image);

	return 1;
}
