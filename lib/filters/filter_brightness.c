/* ----------------------------------------------------------------------
 * filter_brightness
 * parameters[0] = brightness multiplier
 *                 < 1.0 = darken
 *                 > 1.0 = brighten
 *                 1.0 = no change
 * ---------------------------------------------------------------------- */
void
filter_brightness(XImage *img, double parameters[8], struct lock *lock)
{
	if (!img || !img->data) return;

	double factor = parameters[0];
	if (factor <= 0.0) factor = 1.0; /* safety fallback */

	int bpp    = (img->bits_per_pixel + 7) / 8;
	int stride = img->bytes_per_line;

	for (int y = 0; y < img->height; y++) {
		unsigned char *row = (unsigned char *)img->data + y * stride;
		for (int x = 0; x < img->width; x++) {
			unsigned char *px = row + x * bpp;

			int b = (int)(px[0] * factor);
			int g = (int)(px[1] * factor);
			int r = (int)(px[2] * factor);

			if (r > 255) r = 255;
			if (g > 255) g = 255;
			if (b > 255) b = 255;

			px[2] = (unsigned char)r;
			px[1] = (unsigned char)g;
			px[0] = (unsigned char)b;
		}
	}
}
