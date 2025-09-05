#include <Imlib2.h>

// Imlib_Image image;
static XImage *image = NULL;
static void effect_pixelate(XImage *img, int block_size);
static void effect_pixelate2(XImage *img, int block_size);
static void effect_pixelate3(XImage *img, int block_size);

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
render_lock_image(Display *dpy, struct lock *lock, XImage *imgx)
{
	// 1. Grab screenshot into client-side XImage
	XImage *img = XGetImage(dpy, lock->root, 0, 0,
	                        DisplayWidth(dpy, lock->screen),
	                        DisplayHeight(dpy, lock->screen),
	                        AllPlanes, ZPixmap);

	// 2. Apply your pixel effect (in-place)
	effect_pixelate(img, background_pixel_size);

	// 3. Create a server-side Pixmap (same size/depth as window)
	Pixmap pmap = XCreatePixmap(dpy, lock->win,
	                            img->width, img->height,
	                            DefaultDepth(dpy, lock->screen));

	// 4. Upload modified image into Pixmap
	GC gc = XCreateGC(dpy, pmap, 0, NULL);
	XPutImage(dpy, pmap, gc, img, 0, 0, 0, 0, img->width, img->height);

	// 5. Copy Pixmap into your window
	XCopyArea(dpy, pmap, lock->win, lock->gc,
	          0, 0, img->width, img->height, 0, 0);
	XFlush(dpy);

	// cleanup
	XFreeGC(dpy, gc);
	XFreePixmap(dpy, pmap);
	XDestroyImage(img);


// 	XImage *image, *image2, *img;
// 	// GC gc = XCreateGC(dpy, lock->win, 0, NULL);
// 		fprintf(stderr, "create lock image\n");
// 	// if (background_image && strlen(background_image)) {
// 	// 	if (load_lock_image_from_file(dpy, background_image))
// 	// 		return;
// 	// }

// 	image = XGetImage(dpy, lock->root, 0, 0, lock->mw, lock->mh, AllPlanes, ZPixmap);
// 	image2 = XCreateImage(dpy,
//     DefaultVisual(dpy, lock->screen),
//     DefaultDepth(dpy, lock->screen),
//     ZPixmap, 0, NULL,
//     lock->x, lock->y, 32, 0);
// image2->data = malloc(image->bytes_per_line * image->height);

// img = image;
// fprintf(stderr, "copy: depth=%d bpp=%d bitmap_unit=%d bitmap_pad=%d byte_order=%d\n",
//         img->depth, img->bits_per_pixel,
//         img->bitmap_unit, img->bitmap_pad, img->byte_order);

// img = image2;
// fprintf(stderr, "new: depth=%d bpp=%d bitmap_unit=%d bitmap_pad=%d byte_order=%d\n",
//         img->depth, img->bits_per_pixel,
//         img->bitmap_unit, img->bitmap_pad, img->byte_order);

// 	fprintf(stderr, "getimage w = %d, h = %d\n", image->width, image->height);
// for (int y = 0; y < image2->height; y++) {
//     for (int x = 0; x < image2->width; x++) {
//     	// fprintf(stderr, "y = %d, x = %d\n", y, x);
//         XPutPixel(image2, x, y, 0xff0000); // pure red
//     }
// }
// // unsigned long px = XGetPixel(image, 100, 100);
// // fprintf(stderr, "pixel(100,100) = %#lx\n", px);
// // fprintf(stderr, "bpp = %d, bytes_per_line = %d\n",
//         // image->bits_per_pixel, image->bytes_per_line);
// 	// unsigned long px_before = XGetPixel(image, 100, 100);
// effect_pixelate3(image, 12);
// effect_pixelate2(image, 12);
// effect_pixelatec(image, 12);
// // unsigned long px_after = XGetPixel(image, 100, 100);
// // fprintf(stderr, "Pixel before=%#lx after=%#lx\n", px_before, px_after);

// fprintf(stderr, "   image->width = %d\n", image->width);
// fprintf(stderr, "   image->height = %d\n", image->height);
// fprintf(stderr, "   image->depth = %d\n", image->depth);

// fprintf(stderr, "   screen w = %d\n", DisplayWidth(dpy, lock->screen));
// fprintf(stderr, "   screen h = %d\n", DisplayHeight(dpy, lock->screen));
// fprintf(stderr, "   screen depth = %d\n", DefaultDepth(dpy, lock->screen));
// fprintf(stderr, "   lock != NULL ? %d\n", lock != NULL);
// fprintf(stderr, "   lock->win = %ld\n", lock->win);
// fprintf(stderr, "   lock->gc == NULL ? %d\n", lock->gc == NULL);
// fprintf(stderr, "   image != NULL ? %d\n", image != NULL);


// 	XPutImage(dpy, lock->win, lock->gc, image2, 0, 0, lock->x, lock->y, image->width, image->height);
// XFlush(dpy);
// 	XSync(dpy, False);
// 	XDestroyImage(image);
// 	// if (image) {
// 	// 	lock->bgmap = XCreatePixmap(dpy, lock->root, DisplayWidth(dpy, lock->screen), DisplayHeight(dpy, lock->screen), DefaultDepth(dpy, lock->screen));
// 	// 	if (!background_image || !strlen(background_image)) {
// 	// 		imlib_context_set_image(image);
// 	// 	}
// 	// 	imlib_context_set_display(dpy);
// 	// 	imlib_context_set_visual(DefaultVisual(dpy, lock->screen));
// 	// 	imlib_context_set_colormap(DefaultColormap(dpy, lock->screen));
// 	// 	imlib_context_set_drawable(lock->bgmap);
// 	// 	imlib_render_image_on_drawable(0, 0);
// 	// 	imlib_free_image();
// 	// }
// 	fprintf(stderr, "u\n");
}

void
create_lock_image(Display *dpy, struct lock *lock)
{


	return;

	/* Create screenshot Image */
//	Screen *scr = ScreenOfDisplay(dpy, DefaultScreen(dpy));
//	image = imlib_create_image(scr->width,scr->height);
//	imlib_context_set_image(image);
//	imlib_context_set_display(dpy);
//	imlib_context_set_visual(DefaultVisual(dpy,0));
//	imlib_context_set_drawable(RootWindow(dpy,XScreenNumberOfScreen(scr)));
//	imlib_copy_drawable_to_image(0,0,0,scr->width,scr->height,0,0,1);
//
//	/* Pixelation */
//	if (background_pixel_size) {
//		int width = scr->width;
//		int height = scr->height;
//		int pixel_size = background_pixel_size;
//
//		for (int y = 0; y < height; y += pixel_size) {
//			for (int x = 0; x < width; x += pixel_size) {
//				int red = 0;
//				int green = 0;
//				int blue = 0;
//
//				Imlib_Color pixel;
//				Imlib_Color* pp;
//				pp = &pixel;
//				for (int j = 0; j < pixel_size && j < height; j++) {
//					for (int i = 0; i < pixel_size && i < width; i++) {
//						imlib_image_query_pixel(x + i, y + j, pp);
//						red += pixel.red;
//						green += pixel.green;
//						blue += pixel.blue;
//					}
//				}
//				red /= (pixel_size * pixel_size);
//				green /= (pixel_size * pixel_size);
//				blue /= (pixel_size * pixel_size);
//				imlib_context_set_color(red, green, blue, pixel.alpha);
//				imlib_image_fill_rectangle(x, y, pixel_size, pixel_size);
//				red = 0;
//				green = 0;
//				blue = 0;
//			}
//		}
//	} else {
//		/* Blur function */
//		imlib_image_blur(background_blur_radius);
//	}
}

void effect_pixelate3(XImage *img, int block) {
    int bpp = img->bits_per_pixel / 8;   // should be 4
    char *data = img->data;

    for (int y = 0; y < img->height; y += block) {
        for (int x = 0; x < img->width; x += block) {
            // Get pointer to top-left pixel in block
            char *p = data + y * img->bytes_per_line + x * bpp;

            // Copy that pixel into a local buffer
            unsigned char ref[4];
            memcpy(ref, p, bpp);

            // Fill the block with that pixel
            for (int yy = y; yy < y + block && yy < img->height; yy++) {
                char *row = data + yy * img->bytes_per_line;
                for (int xx = x; xx < x + block && xx < img->width; xx++) {
                    memcpy(row + xx * bpp, ref, bpp);
                }
            }
        }
    }
}

void effect_pixelate2(XImage *img, int block) {
    int bpp = img->bits_per_pixel / 8;
    for (int y = 0; y < img->height; y += block) {
        for (int x = 0; x < img->width; x += block) {
            // Sample top-left pixel in block
            unsigned long pixel = XGetPixel(img, x, y);

            // Fill the block
            for (int yy = y; yy < y + block && yy < img->height; yy++) {
                for (int xx = x; xx < x + block && xx < img->width; xx++) {
                    XPutPixel(img, xx, yy, pixel);
                }
            }
        }
    }
}

// Apply pixelation to XImage in place
void
effect_pixelate(XImage *img, int block_size)
{
    int width  = img->width;
    int height = img->height;
    int bpp    = img->bits_per_pixel / 8;
    int pitch  = img->bytes_per_line;

    for (int by = 0; by < height; by += block_size) {
        for (int bx = 0; bx < width; bx += block_size) {
            int rsum = 0, gsum = 0, bsum = 0, count = 0;

            // 1. Compute block average
            for (int y = by; y < by + block_size && y < height; y++) {
                uint8_t *row = (uint8_t *)img->data + y * pitch;
                for (int x = bx; x < bx + block_size && x < width; x++) {
                    uint8_t *px = row + x * bpp;
                    bsum += px[0];
                    gsum += px[1];
                    rsum += px[2];
                    count++;
                }
            }

            if (count == 0) continue;
            uint8_t r = rsum / count;
            uint8_t g = gsum / count;
            uint8_t b = bsum / count;

            // 2. Fill block with average color
            for (int y = by; y < by + block_size && y < height; y++) {
                uint8_t *row = (uint8_t *)img->data + y * pitch;
                for (int x = bx; x < bx + block_size && x < width; x++) {
                    uint8_t *px = row + x * bpp;
                    px[0] = b;
                    px[1] = g;
                    px[2] = r;
                }
            }
        }
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
