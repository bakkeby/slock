#include <Imlib2.h>
#include <math.h>

// Imlib_Image image;
static XImage *image = NULL;
static void effect_pixelate(XImage *img, int block_size);
static void effect_pixelate2(XImage *img, int block_size);
static void effect_pixelate3(XImage *img, int block_size);
static void effect_rain_shift(XImage *img, int max_shift);
static void rain_shift_vert(XImage *img, int max_shift);
static void rain_shift_horiz(XImage *img, int max_shift);
static void add_noise(XImage *img, double amount);
static void effect_blur(XImage *img, int radius);
static void add_soft_noise(XImage *img,
                           double amount,
                           double strength,
                           int    blur_radius);
static int img_to_grayscale(XImage *img);
static int dual_kawase_blur(XImage *img, int iterations, int radius);

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
	// The below is in the context of imlib2 Image and converting that to a pixmap
	// if (imgx) {
	// 	lock->bgmap = XCreatePixmap(dpy, lock->root, DisplayWidth(dpy, lock->screen), DisplayHeight(dpy, lock->screen), DefaultDepth(dpy, lock->screen));
	// 	if (!background_image || !strlen(background_image)) {
	// 		imlib_context_set_image(image);
	// 	}
	// 	imlib_context_set_display(dpy);
	// 	imlib_context_set_visual(DefaultVisual(dpy, lock->screen));
	// 	imlib_context_set_colormap(DefaultColormap(dpy, lock->screen));
	// 	imlib_context_set_drawable(lock->bgmap);
	// 	imlib_render_image_on_drawable(0, 0);
	// 	imlib_free_image();
	// }

	// 4. Upload modified image into Pixmap
	// GC gc = XCreateGC(dpy, pmap, 0, NULL);
	// XPutImage(dpy, pmap, gc, img, 0, 0, 0, 0, img->width, img->height);

// 	// 5. Copy Pixmap into your window
// 	XCopyArea(dpy, pmap, lock->win, lock->gc,
// 	          0, 0, img->width, img->height, 0, 0);
// fprintf(stderr, "x\n");
// 	XFlush(dpy);

// 	// cleanup
// 	XFreeGC(dpy, gc);
// 	XFreePixmap(dpy, pmap);
// 	XDestroyImage(img);


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

static inline uint8_t clamp255(int v) {
    return v < 0 ? 0 : (v > 255 ? 255 : v);
}

void effect_posterize(XImage *img, int levels) {
    int width  = img->width;
    int height = img->height;
    int bpp    = img->bits_per_pixel / 8;   // should be 4
    int stride = img->bytes_per_line;

    if (levels < 2) levels = 2;  // at least 2 levels
    int step = 256 / levels;

    uint8_t *data = (uint8_t *)img->data;

    for (int y = 0; y < height; y++) {
        uint8_t *row = data + y * stride;
        for (int x = 0; x < width; x++) {
            uint8_t *p = row + x * bpp;

            // Posterize each channel
            p[0] = clamp255((p[0] / step) * step);  // Blue
            p[1] = clamp255((p[1] / step) * step);  // Green
            p[2] = clamp255((p[2] / step) * step);  // Red
            // Leave p[3] (padding/alpha) unchanged
        }
    }
}

void effect_sobel(XImage *img) {
    int width  = img->width;
    int height = img->height;
    int bpp    = img->bits_per_pixel / 8; // 4
    int stride = img->bytes_per_line;

    uint8_t *data = (uint8_t *)img->data;
    uint8_t *tmp = malloc(stride * height);
    memcpy(tmp, data, stride * height);  // preserve original

    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        { 0,  0,  0},
        { 1,  2,  1}
    };

    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int sumRx = 0, sumGx = 0, sumBx = 0;
            int sumRy = 0, sumGy = 0, sumBy = 0;

            // Apply 3x3 Sobel kernel
            for (int ky = -1; ky <= 1; ky++) {
                uint8_t *row = tmp + (y + ky) * stride;
                for (int kx = -1; kx <= 1; kx++) {
                    uint8_t *p = row + (x + kx) * bpp;
                    int kxval = Gx[ky + 1][kx + 1];
                    int kyval = Gy[ky + 1][kx + 1];

                    sumBx += p[0] * kxval;
                    sumGx += p[1] * kxval;
                    sumRx += p[2] * kxval;

                    sumBy += p[0] * kyval;
                    sumGy += p[1] * kyval;
                    sumRy += p[2] * kyval;
                }
            }

            uint8_t *dst = data + y * stride + x * bpp;
            dst[0] = clamp255((int)sqrt(sumBx * sumBx + sumBy * sumBy));
            dst[1] = clamp255((int)sqrt(sumGx * sumGx + sumGy * sumGy));
            dst[2] = clamp255((int)sqrt(sumRx * sumRx + sumRy * sumRy));
            // keep dst[3] as is
        }
    }

    free(tmp);
}


void effect_invert(XImage *img) {
    int width  = img->width;
    int height = img->height;
    int bpp    = img->bits_per_pixel / 8;   // 4
    int stride = img->bytes_per_line;

    uint8_t *data = (uint8_t *)img->data;

    for (int y = 0; y < height; y++) {
        uint8_t *row = data + y * stride;
        for (int x = 0; x < width; x++) {
            uint8_t *p = row + x * bpp;

            p[0] = 255 - p[0];  // Blue
            p[1] = 255 - p[1];  // Green
            p[2] = 255 - p[2];  // Red
            // Leave p[3] (padding/alpha) unchanged
        }
    }
}

void effect_scanlines(XImage *img, int spacing, float intensity) {
    int width  = img->width;
    int height = img->height;
    int bpp    = img->bits_per_pixel / 8;
    int stride = img->bytes_per_line;
    uint8_t *data = (uint8_t *)img->data;

    for (int y = 0; y < height; y += spacing) {
        uint8_t *row = data + y * stride;
        for (int x = 0; x < width; x++) {
            uint8_t *p = row + x * bpp;
            p[0] = (uint8_t)(p[0] * intensity); // B
            p[1] = (uint8_t)(p[1] * intensity); // G
            p[2] = (uint8_t)(p[2] * intensity); // R
        }
    }
}

void effect_chroma_shift(XImage *img, int shift) {
    int width  = img->width;
    int height = img->height;
    int bpp    = img->bits_per_pixel / 8;
    int stride = img->bytes_per_line;
    uint8_t *data = (uint8_t *)img->data;

    for (int y = 0; y < height; y++) {
        uint8_t *row = data + y * stride;
        for (int x = width - 1; x >= shift; x--) {
            uint8_t *dst = row + x * bpp;
            uint8_t *srcR = row + (x - shift) * bpp;
            dst[2] = srcR[2]; // shift red channel
        }
        // green/blue could be shifted differently for effect
    }
}

void effect_noise(XImage *img, float amount) {
    int width  = img->width;
    int height = img->height;
    int bpp    = img->bits_per_pixel / 8;
    int stride = img->bytes_per_line;
    uint8_t *data = (uint8_t *)img->data;

    for (int y = 0; y < height; y++) {
        uint8_t *row = data + y * stride;
        for (int x = 0; x < width; x++) {
            uint8_t *p = row + x * bpp;
            int r = p[2] + (rand() % (int)(255*amount)) - (int)(127*amount);
            int g = p[1] + (rand() % (int)(255*amount)) - (int)(127*amount);
            int b = p[0] + (rand() % (int)(255*amount)) - (int)(127*amount);
            p[2] = r < 0 ? 0 : (r > 255 ? 255 : r);
            p[1] = g < 0 ? 0 : (g > 255 ? 255 : g);
            p[0] = b < 0 ? 0 : (b > 255 ? 255 : b);
        }
    }
}
void effect_vhs_jitter(XImage *img, int max_offset) {
    int width  = img->width;
    int height = img->height;
    int bpp    = img->bits_per_pixel / 8;
    int stride = img->bytes_per_line;
    uint8_t *data = (uint8_t *)img->data;

    uint8_t *tmp = malloc(stride * height);
    memcpy(tmp, data, stride * height);

    for (int y = 0; y < height; y++) {
        int offset = (rand() % (2*max_offset + 1)) - max_offset;
        int yy = y + offset;
        if (yy < 0) yy = 0;
        if (yy >= height) yy = height - 1;

        memcpy(data + y * stride, tmp + yy * stride, stride);
    }
    free(tmp);
}


typedef struct {
    int x0, y0;      // region origin
    int w, h;        // region size
    int initialized; // flag
    int damage;      // accumulated damage
} VHSState;

#define MAX_DAMAGE 20   // after this, region resets

void effect_vhs_pixmap(Display *dpy, Drawable pix, GC gc, VHSState *state) {
    // Get geometry of the pixmap
    Window root;
    int x, y;
    unsigned int width, height, border, depth;
    XGetGeometry(dpy, pix, &root, &x, &y, &width, &height, &border, &depth);

    // Grab the pixmap as an XImage
    XImage *img = XGetImage(dpy, pix, 0, 0, width, height, AllPlanes, ZPixmap);
    if (!img) return;

    int bpp    = img->bits_per_pixel / 8;
    int stride = img->bytes_per_line;
    uint8_t *data = (uint8_t *)img->data;

    // Reset region if not initialized or too damaged
    if (!state->initialized || state->damage > MAX_DAMAGE) {
        state->w = width / 5 + rand() % (width / 10);
        state->h = height / 5 + rand() % (height / 10);
        state->x0 = rand() % (width - state->w);
        state->y0 = rand() % (height - state->h);
        state->damage = 0;
        state->initialized = 1;
    }

    // Increase cumulative damage
    state->damage++;

    int x1 = state->x0, y1 = state->y0;
    int x2 = x1 + state->w, y2 = y1 + state->h;
    if (x2 > (int)width)  x2 = width;
    if (y2 > (int)height) y2 = height;

    // Temporary row buffer
    uint8_t *rowbuf = malloc(stride);

    for (int yy = y1; yy < y2; yy++) {
        uint8_t *row = data + yy * stride;

        // Horizontal jitter (stronger with damage)
        int shift = (rand() % (state->damage + 1)) - state->damage / 2;

        memcpy(rowbuf, row + x1 * bpp, (x2 - x1) * bpp);
        for (int xx = x1; xx < x2; xx++) {
            int srcx = xx + shift;
            if (srcx < x1) srcx = x1;
            if (srcx >= x2) srcx = x2 - 1;
            memcpy(row + xx * bpp, rowbuf + (srcx - x1) * bpp, bpp);
        }

        // Add occasional white noise speckles
        if ((rand() % 50) < state->damage) {
            uint8_t *p = row + (x1 + rand() % (x2 - x1)) * bpp;
            p[0] = p[1] = p[2] = 255; // white dot
        }

        // Horizontal dropout line (rare, when damage is higher)
        if ((rand() % 200) < state->damage) {
            memset(row + x1 * bpp, 255, (x2 - x1) * bpp);
        }
    }

    free(rowbuf);

    // Chroma bleed (red channel shifted)
    for (int yy = y1; yy < y2; yy++) {
        uint8_t *row = data + yy * stride;
        for (int xx = x1 + 2; xx < x2; xx++) {
            uint8_t *p  = row + xx * bpp;
            uint8_t *p2 = row + (xx - 2) * bpp;
            p[2] = p2[2]; // shift red
        }
    }

    // Push changes back into the pixmap
    XPutImage(dpy, pix, gc, img, 0, 0, 0, 0, width, height);

    // Free the XImage (XPutImage copies data)
    XDestroyImage(img);
}

void
create_lock_image(Display *dpy, struct lock *lock)
{
	if (background_image && strlen(background_image)) {
		if (load_lock_image_from_file(dpy, background_image))
			return;
	}


	// 1. Grab screenshot into client-side XImage
	XImage *img = XGetImage(dpy, lock->root, 0, 0,
	                        DisplayWidth(dpy, lock->screen),
	                        DisplayHeight(dpy, lock->screen),
	                        AllPlanes, ZPixmap);
	// 2. Apply your pixel effect (in-place)
	// effect_pixelate(img, background_pixel_size);
	// rain_shift_horiz(img, background_pixel_size);
	// rain_shift_vert(img, background_pixel_size);
	// effect_blur(img, background_blur_radius);
	// dual_kawase_blur(img, 3, 3);
	// effect_posterize(img, background_pixel_size);
	// effect_sobel(img);
	// effect_invert(img);
	// img_to_grayscale(img);
	// effect_scanlines(img, 5, 0.35);
	// effect_chroma_shift(img, 5);
	// effect_noise(img, 2);
	// effect_vhs_jitter(img, 8);

effect_blur(img, 2);
effect_scanlines(img, 2, 0.7f);
effect_chroma_shift(img, 2);
effect_noise(img, 0.15f);
effect_vhs_jitter(img, 3);

// amount	0.01 – 0.15	More pixels become noisy. Small values give a subtle film‑grain look; larger values turn the image into a “static” overlay.
// strength	0.03 – 0.20	Controls how far the colour can wander from the original. Low values keep the grain close to the underlying tones, yielding a soft appearance.
// blur_radius	0 – 3 (integer)	A radius of 1‑2 smooths the speckles nicely without washing out detail. Larger radii start to look like a mild Gaussian blur over the whole picture.

// add_soft_noise(img, 0.10, 0.15, 2.5);
	// 3. Create a server-side Pixmap (same size/depth as window)
	lock->bgmap = XCreatePixmap(dpy, lock->win,
	                            img->width, img->height,
	                            DefaultDepth(dpy, lock->screen));
	XPutImage(dpy, lock->bgmap, lock->gc, img,
	          0, 0, 0, 0, img->width, img->height);

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
    // int bpp = img->bits_per_pixel / 8;
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


/* -------------------------------------------------------------
   rain_shift_image()
   -------------------------------------------------------------
   Parameters
     XImage *img      – the XImage you got from XGetImage().
                       It must be a ZPixmap image (the usual case).
     int max_shift   – maximum number of rows a column may be
                       shifted downwards (e.g. 10‑30 gives a
                       gentle rain; larger values look more
                       dramatic).  The shift for each column is
                       chosen independently and uniformly at
                       random in the range [0, max_shift].

   What it does
     • For every x‑coordinate (column) we pick a random offset.
     • Pixels that fall off the bottom are wrapped to the top,
       which creates a continuous “stream” effect.
     • The manipulation happens in‑place on img->data, so no
       extra buffers are required (except a temporary row
       buffer used during the per‑column copy).

   Note
     • The routine assumes the image depth is a multiple of 8
       bits (most modern X servers use 24‑ or 32‑bit depth).
     • If your server uses a non‑byte‑aligned depth you would
       need to adjust the byte‑per‑pixel calculation.
   ------------------------------------------------------------- */


void
effect_rain_shift(XImage *img, int max_shift)
{
    if (!img || max_shift <= 0) return;

    /* Seed the PRNG once – you can move this elsewhere if you
       already seed your program. */
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    /* Bytes per pixel – works for 24‑bpp (3) and 32‑bpp (4). */
    int bpp = img->bits_per_pixel / 8;
    if (bpp == 0) bpp = 4;               /* safety fallback */

    /* Allocate a temporary buffer that holds one column. */
    unsigned char *col_buf = malloc(img->height * bpp);
    if (!col_buf) return;                /* out‑of‑memory guard */

    for (int x = 0; x < img->width; ++x) {
        /* 1️⃣  Copy the whole column into col_buf */
        for (int y = 0; y < img->height; ++y) {
            unsigned char *src = (unsigned char *)img->data +
                                 y * img->bytes_per_line + x * bpp;
            memcpy(col_buf + y * bpp, src, bpp);
        }

        /* 2️⃣  Choose a random vertical offset for this column */
        int offset = rand() % (max_shift + 1);   /* 0 … max_shift */

        /* 3️⃣  Write the column back, shifted downwards.
            Pixels that would fall off the bottom wrap to the top. */
        for (int y = 0; y < img->height; ++y) {
            int src_y = (y - offset + img->height) % img->height;
            unsigned char *dst = (unsigned char *)img->data +
                                 y * img->bytes_per_line + x * bpp;
            memcpy(dst, col_buf + src_y * bpp, bpp);
        }
    }

    free(col_buf);
}

/* -------------------------------------------------------------
   rain_shift_vert()
   -------------------------------------------------------------
   Shift each column downward by a random offset in the range
   [0 .. max_shift].  Pixels that fall off the bottom wrap to the
   top.  The operation is performed in‑place on the XImage.
   ------------------------------------------------------------- */
static void rain_shift_vert(XImage *img, int max_shift)
{
    if (!img || max_shift <= 0) return;

    static int seeded = 0;
    if (!seeded) { srand((unsigned)time(NULL)); seeded = 1; }

    int bpp = img->bits_per_pixel / 8;          /* bytes per pixel */
    if (bpp == 0) bpp = 4;                      /* safety fallback */

    unsigned char *col_buf = malloc(img->height * bpp);
    if (!col_buf) return;                       /* OOM guard */

    for (int x = 0; x < img->width; ++x) {
        /* copy column into temporary buffer */
        for (int y = 0; y < img->height; ++y) {
            unsigned char *src = (unsigned char *)img->data +
                                 y * img->bytes_per_line + x * bpp;
            memcpy(col_buf + y * bpp, src, bpp);
        }

        int offset = rand() % (max_shift + 1);   /* 0 … max_shift */

        /* write column back, shifted downwards (wrap around) */
        for (int y = 0; y < img->height; ++y) {
            int src_y = (y - offset + img->height) % img->height;
            unsigned char *dst = (unsigned char *)img->data +
                                 y * img->bytes_per_line + x * bpp;
            memcpy(dst, col_buf + src_y * bpp, bpp);
        }
    }
    free(col_buf);
}

/* -------------------------------------------------------------
   rain_shift_horiz()
   -------------------------------------------------------------
   Shift each row left/right by a random offset in the range
   [0 .. max_shift].  Positive offsets move the row to the right,
   negative offsets move it to the left.  Wrapping is performed so
   no pixels are lost.  The operation is performed in‑place on the
   XImage.
   ------------------------------------------------------------- */
static void rain_shift_horiz(XImage *img, int max_shift)
{
    if (!img || max_shift <= 0) return;

    static int seeded = 0;
    if (!seeded) { srand((unsigned)time(NULL)); seeded = 1; }

    int bpp = img->bits_per_pixel / 8;          /* bytes per pixel */
    if (bpp == 0) bpp = 4;                      /* safety fallback */

    /* Temporary buffer that holds one full row */
    unsigned char *row_buf = malloc(img->width * bpp);
    if (!row_buf) return;                       /* OOM guard */

    for (int y = 0; y < img->height; ++y) {
        /* copy the whole row into row_buf */
        unsigned char *src_row = (unsigned char *)img->data +
                                 y * img->bytes_per_line;
        memcpy(row_buf, src_row, img->width * bpp);

        /* pick a random horizontal offset: 0 … max_shift */
        int offset = rand() % (max_shift + 1);

        /* optionally randomise direction (left/right) */
        if (rand() & 1) offset = -offset;      /* 50 % left, 50 % right */

        /* write the row back, shifted with wrap‑around */
        for (int x = 0; x < img->width; ++x) {
            int src_x = (x - offset + img->width) % img->width;
            unsigned char *dst = src_row + x * bpp;
            memcpy(dst, row_buf + src_x * bpp, bpp);
        }
    }
    free(row_buf);
}

static void add_noise(XImage *img, double amount)
{
    if (!img) return;
    if (amount <= 0.0) return;          /* nothing to do */
    if (amount > 1.0) amount = 1.0;     /* clamp */

    /* Seed the PRNG once – you can move this elsewhere if you
       already seed your program. */
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    /* Bytes per pixel – works for 24‑bpp (3) and 32‑bpp (4). */
    int bpp = img->bits_per_pixel / 8;
    if (bpp == 0) bpp = 4;               /* safety fallback */

    /* Total number of pixels in the image */
    const long total_pixels = (long)img->width * (long)img->height;

    /* How many pixels we will corrupt */
    long n_noise = (long)(total_pixels * amount + 0.5);
    if (n_noise == 0) n_noise = 1;      /* guarantee at least one */

    for (long i = 0; i < n_noise; ++i) {
        /* Pick a random pixel coordinate */
        int x = rand() % img->width;
        int y = rand() % img->height;
        /* Build a random colour value.
           For 24‑bpp we generate 3 bytes (RGB).  For 32‑bpp we also
           generate an unused alpha byte – most X servers ignore it. */
        uint32_t rnd = ((uint32_t)rand() << 16) | (uint32_t)rand();
        unsigned char *dst = (unsigned char *)img->data +
                             y * img->bytes_per_line + x * bpp;

        /* Copy the low‑order bytes into the pixel */
        for (int k = 0; k < bpp; ++k) {
            dst[k] = (unsigned char)(rnd & 0xFF);
            rnd >>= 8;
        }
    }
}

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void effect_blur(XImage *img, int radius) {
    int width  = img->width;
    int height = img->height;
    int bpp    = img->bits_per_pixel / 8;  // 4
    int stride = img->bytes_per_line;

    // allocate temporary buffer
    uint8_t *tmp = malloc(stride * height);
    memcpy(tmp, img->data, stride * height);  // start with original

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int rsum = 0, gsum = 0, bsum = 0, count = 0;

            for (int dy = -radius; dy <= radius; dy++) {
                int yy = y + dy;
                if (yy < 0 || yy >= height) continue;

                uint8_t *row = (uint8_t *)img->data + yy * stride;
                for (int dx = -radius; dx <= radius; dx++) {
                    int xx = x + dx;
                    if (xx < 0 || xx >= width) continue;

                    uint8_t *p = row + xx * bpp;
                    bsum += p[0];  // B
                    gsum += p[1];  // G
                    rsum += p[2];  // R
                    count++;
                }
            }

            // write averaged value to temporary buffer
            uint8_t *dst = tmp + y * stride + x * bpp;
            dst[0] = bsum / count;
            dst[1] = gsum / count;
            dst[2] = rsum / count;
            if (bpp == 4) dst[3] = 0;  // preserve alpha/padding
        }
    }

    // copy blurred buffer back to image
    memcpy(img->data, tmp, stride * height);
    free(tmp);
}

/* -------------------------------------------------------------
   Helper: fetch a pixel from a buffer (no bounds check)
   ------------------------------------------------------------- */
static inline void get_pixel(const unsigned char *buf,
                             int stride, int bpp,
                             int x, int y,
                             unsigned char out[4])
{
    const unsigned char *p = buf + y * stride + x * bpp;
    for (int i = 0; i < bpp; ++i) out[i] = p[i];
}

/* Helper: write a pixel into a buffer */
static inline void set_pixel(unsigned char *buf,
                             int stride, int bpp,
                             int x, int y,
                             const unsigned char in[4])
{
    unsigned char *p = buf + y * stride + x * bpp;
    for (int i = 0; i < bpp; ++i) p[i] = in[i];
}

/* -------------------------------------------------------------
   dual_kawase_blur()
   ------------------------------------------------------------- */
int dual_kawase_blur(XImage *img, int iterations, int radius)
{
    if (!img || iterations <= 0 || radius < 1) return -1;

    /* ---------------------------------------------------------
       Determine bytes‑per‑pixel (24‑bpp => 3, 32‑bpp => 4)
       --------------------------------------------------------- */
    int bpp = img->bits_per_pixel / 8;
    if (bpp < 3) return -1;               /* unsupported depth */

    /* ---------------------------------------------------------
       Allocate two auxiliary buffers that will hold the
       intermediate down‑sampled image.  We allocate the *largest*
       possible size (original width/height) for simplicity and
       reuse the same memory for every iteration.
       --------------------------------------------------------- */
    size_t buf_sz = (size_t)img->height * img->bytes_per_line;
    unsigned char *tmp0 = malloc(buf_sz);
    unsigned char *tmp1 = malloc(buf_sz);
    if (!tmp0 || !tmp1) { free(tmp0); free(tmp1); return -1; }

    /* ---------------------------------------------------------
       Copy the original image into tmp0 – this becomes our
       working source for the first down‑sample pass.
       --------------------------------------------------------- */
    memcpy(tmp0, img->data, buf_sz);

    int src_w = img->width;
    int src_h = img->height;
    int src_stride = img->bytes_per_line;

    for (int iter = 0; iter < iterations; ++iter) {
        /* ---------- 1️⃣ Down‑sample pass (tmp0 -> tmp1) ---------- */
        int dst_w = src_w > 1 ? src_w / 2 : 1;
        int dst_h = src_h > 1 ? src_h / 2 : 1;
        int dst_stride = src_stride;   /* keep same stride for simplicity */

        for (int y = 0; y < dst_h; ++y) {
            for (int x = 0; x < dst_w; ++x) {
                /* Source coordinates in the higher‑res buffer */
                int sx = x * 2;
                int sy = y * 2;

                /* Accumulate centre + 4 orthogonal neighbours */
                unsigned int sum[4] = {0,0,0,0};
                unsigned char pix[4];

                /* centre */
                get_pixel(tmp0, src_stride, bpp, sx, sy, pix);
                for (int c = 0; c < bpp; ++c) sum[c] += pix[c];

                /* left */
                if (sx - radius >= 0) {
                    get_pixel(tmp0, src_stride, bpp, sx - radius, sy, pix);
                    for (int c = 0; c < bpp; ++c) sum[c] += pix[c];
                }

                /* right */
                if (sx + radius < src_w) {
                    get_pixel(tmp0, src_stride, bpp, sx + radius, sy, pix);
                    for (int c = 0; c < bpp; ++c) sum[c] += pix[c];
                }

                /* top */
                if (sy - radius >= 0) {
                    get_pixel(tmp0, src_stride, bpp, sx, sy - radius, pix);
                    for (int c = 0; c < bpp; ++c) sum[c] += pix[c];
                }

                /* bottom */
                if (sy + radius < src_h) {
                    get_pixel(tmp0, src_stride, bpp, sx, sy + radius, pix);
                    for (int c = 0; c < bpp; ++c) sum[c] += pix[c];
                }

                /* Average (5 taps max) */
                unsigned char out[4];
                for (int c = 0; c < bpp; ++c) out[c] = (unsigned char)(sum[c] / 5);
                set_pixel(tmp1, dst_stride, bpp, x, y, out);
            }
        }

        /* ---------- 2️⃣ Up‑sample pass (tmp1 -> tmp0) ---------- */
        /* Now tmp1 holds the down‑scaled image (dst_w × dst_h). */
        /* We up‑sample back to src_w × src_h. */
        for (int y = 0; y < src_h; ++y) {
            for (int x = 0; x < src_w; ++x) {
                /* Corresponding coordinate in the low‑res buffer */
                int lx = x / 2;
                int ly = y / 2;

                /* Accumulate centre + 4 neighbours in low‑res space */
                unsigned int sum[4] = {0,0,0,0};
                unsigned char pix[4];

                /* centre */
                get_pixel(tmp1, dst_stride, bpp, lx, ly, pix);
                for (int c = 0; c < bpp; ++c) sum[c] += pix[c];

                /* left */
                if (lx - radius >= 0) {
                    get_pixel(tmp1, dst_stride, bpp, lx - radius, ly, pix);
                    for (int c = 0; c < bpp; ++c) sum[c] += pix[c];
                }

                /* right */
                if (lx + radius < dst_w) {
                    get_pixel(tmp1, dst_stride, bpp, lx + radius, ly, pix);
                    for (int c = 0; c < bpp; ++c) sum[c] += pix[c];
                }

                /* top */
                if (ly - radius >= 0) {
                    get_pixel(tmp1, dst_stride, bpp, lx, ly - radius, pix);
                    for (int c = 0; c < bpp; ++c) sum[c] += pix[c];
                }

                /* bottom */
                if (ly + radius < dst_h) {
                    get_pixel(tmp1, dst_stride, bpp, lx, ly + radius, pix);
                    for (int c = 0; c < bpp; ++c) sum[c] += pix[c];
                }

                unsigned char out[4];
                for (int c = 0; c < bpp; ++c) out[c] = (unsigned char)(sum[c] / 5);
                set_pixel(tmp0, src_stride, bpp, x, y, out);
            }
        }

        /* Reduce the radius a little after each iteration – this
           mimics the classic Dual‑Kawase cascade and prevents
           overshooting. */
        if (radius > 1) radius--;
    }

    /* ---------------------------------------------------------
       Copy the final blurred buffer back into the XImage.
       --------------------------------------------------------- */
    memcpy(img->data, tmp0, buf_sz);

    free(tmp0);
    free(tmp1);
    return 0;
}

/* -------------------------------------------------------------
   img_to_grayscale()
   -------------------------------------------------------------
   Parameters
     XImage *img   – image returned by XGetImage().
   What it does
     • Walks through every pixel of the image.
     • Computes a luminance value using the Rec. 601 coefficients
       (R × 0.299 + G × 0.587 + B × 0.114).  Those weights give a
       perceptually‑balanced gray that matches what most viewers
       expect.
     • Replaces the R, G and B components with that luminance.
       If the image has an extra alpha byte (32‑bpp) the alpha is
       left untouched.
   Returns
     0 on success, –1 if the image depth is unsupported.
   ------------------------------------------------------------- */
#include <stddef.h>   /* size_t */

static int img_to_grayscale(XImage *img)
{
    if (!img) return -1;

    /* Bytes per pixel – works for 24‑bpp (3) and 32‑bpp (4). */
    int bpp = img->bits_per_pixel / 8;
    if (bpp < 3) return -1;          /* we need at least RGB */

    /* Walk the image row‑by‑row */
    for (int y = 0; y < img->height; ++y) {
        unsigned char *row = (unsigned char *)img->data +
                             y * img->bytes_per_line;

        for (int x = 0; x < img->width; ++x) {
            unsigned char *pix = row + x * bpp;

            /* XImage stores pixels in native byte order:
               most X servers use little‑endian, so the order is
               B G R (and possibly A).  To be safe we read the
               three colour bytes individually. */
            unsigned char blue  = pix[0];
            unsigned char green = pix[1];
            unsigned char red   = pix[2];

            /* Rec. 601 luma = 0.299 R + 0.587 G + 0.114 B */
            unsigned char gray = (unsigned char)(
                0.299f * red +
                0.587f * green +
                0.114f * blue + 0.5f);   /* +0.5 for rounding */

            /* Write the gray value back to all colour channels */
            pix[0] = gray;   /* B */
            pix[1] = gray;   /* G */
            pix[2] = gray;   /* R */
            /* Alpha (pix[3]) is left unchanged if it exists */
        }
    }
    return 0;
}

static void add_soft_noise(XImage *img,
                           double amount,
                           double strength,
                           int    blur_radius)
{
    if (!img) return;
    if (amount <= 0.0) return;
    if (strength <= 0.0) strength = 0.01;   /* avoid division by zero */
    if (strength > 1.0) strength = 1.0;
    if (blur_radius < 0) blur_radius = 0;

    /* ---------------------------------------------------------
       1️⃣  Initialise PRNG (once per process)
       --------------------------------------------------------- */
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned)time(NULL));
        seeded = 1;
    }

    /* ---------------------------------------------------------
       2️⃣  Determine bytes‑per‑pixel (works for 24‑ and 32‑bpp)
       --------------------------------------------------------- */
    int bpp = img->bits_per_pixel / 8;
    if (bpp == 0) bpp = 4;                 /* safety fallback */

    const long total_pixels = (long)img->width * (long)img->height;
    long n_noise = (long)(total_pixels * amount + 0.5);
    if (n_noise == 0) n_noise = 1;         /* at least one pixel */

    /* ---------------------------------------------------------
       3️⃣  Add noise – each affected pixel receives a colour
           that is a weighted blend of its original value and a
           random offset limited by `strength`.
       --------------------------------------------------------- */
    for (long i = 0; i < n_noise; ++i) {
        int x = rand() % img->width;
        int y = rand() % img->height;

        unsigned char *pix = (unsigned char *)img->data +
                             y * img->bytes_per_line + x * bpp;

        /* Original colour components (0‑255) */
        unsigned char orig[4];
        for (int c = 0; c < bpp; ++c) orig[c] = pix[c];

        /* Random offset limited to ±strength*255 */
        int max_delta = (int)(strength * 255.0);
        for (int c = 0; c < bpp; ++c) {
            int delta = (rand() % (2 * max_delta + 1)) - max_delta;
            int val   = (int)orig[c] + delta;
            if (val <   0) val =   0;
            if (val > 255) val = 255;
            pix[c] = (unsigned char)val;
        }
    }

    /* ---------------------------------------------------------
       4️⃣  Optional box‑blur to smooth the high‑frequency spikes.
           We implement a separable blur (horizontal then vertical)
           because it is O(width·height·radius) instead of O(N²).
       --------------------------------------------------------- */
    if (blur_radius > 0) {
        /* Allocate a temporary buffer the size of the image */
        unsigned char *tmp = malloc(img->height * img->bytes_per_line);
        if (!tmp) return;   /* OOM – skip blur */

        /* ----- Horizontal pass ----- */
        for (int y = 0; y < img->height; ++y) {
            unsigned char *src = (unsigned char *)img->data + y * img->bytes_per_line;
            unsigned char *dst = tmp + y * img->bytes_per_line;

            for (int x = 0; x < img->width; ++x) {
                int sum[4] = {0,0,0,0};
                int cnt = 0;
                for (int k = -blur_radius; k <= blur_radius; ++k) {
                    int xx = x + k;
                    if (xx < 0 || xx >= img->width) continue;
                    unsigned char *p = src + xx * bpp;
                    for (int c = 0; c < bpp; ++c) sum[c] += p[c];
                    ++cnt;
                }
                unsigned char *out = dst + x * bpp;
                for (int c = 0; c < bpp; ++c) out[c] = (unsigned char)(sum[c] / cnt);
            }
        }

        /* ----- Vertical pass (write back into img->data) ----- */
        for (int y = 0; y < img->height; ++y) {
            unsigned char *dst = (unsigned char *)img->data + y * img->bytes_per_line;
            for (int x = 0; x < img->width; ++x) {
                int sum[4] = {0,0,0,0};
                int cnt = 0;
                for (int k = -blur_radius; k <= blur_radius; ++k) {
                    int yy = y + k;
                    if (yy < 0 || yy >= img->height) continue;
                    unsigned char *p = tmp + yy * img->bytes_per_line + x * bpp;
                    for (int c = 0; c < bpp; ++c) sum[c] += p[c];
                    ++cnt;
                }
                unsigned char *out = dst + x * bpp;
                for (int c = 0; c < bpp; ++c) out[c] = (unsigned char)(sum[c] / cnt);
            }
        }

        free(tmp);
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
