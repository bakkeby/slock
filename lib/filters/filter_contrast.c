/* ----------------------------------------------------------------------
 * filter_contrast
 * parameters[0] = contrast multiplier
 *                 < 1.0 = lower contrast
 *                 > 1.0 = higher contrast
 *                 1.0 = no change
 * ---------------------------------------------------------------------- */
void
filter_contrast(XImage *img, double parameters[8], struct lock *lock)
{
	if (!img || !img->data) return;

	double factor = parameters[0];
	if (factor <= 0.0) factor = 1.0;

	int bpp    = (img->bits_per_pixel + 7) / 8;
	int stride = img->bytes_per_line;

	for (int y = 0; y < img->height; y++) {
		unsigned char *row = (unsigned char *)img->data + y * stride;
		for (int x = 0; x < img->width; x++) {
			unsigned char *px = row + x * bpp;

			// Center around 128, apply factor, shift back
			int b = (int)(((px[0] - 128) * factor) + 128);
			int g = (int)(((px[1] - 128) * factor) + 128);
			int r = (int)(((px[2] - 128) * factor) + 128);

			if (r < 0) { r = 0; }; if (r > 255) { r = 255; };
			if (g < 0) { g = 0; }; if (g > 255) { g = 255; };
			if (b < 0) { b = 0; }; if (b > 255) { b = 255; };

			px[2] = (unsigned char)r;
			px[1] = (unsigned char)g;
			px[0] = (unsigned char)b;
		}
	}
}
