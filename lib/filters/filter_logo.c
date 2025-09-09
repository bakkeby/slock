void
filter_logo(XImage *img, double parameters[8], struct lock *lock)
{
	if (!img || !img->data | !lock->rectangles)
		return;

	int fg = lock->colors[CLAMP((int)parameters[0], 0, NUMCOLS - 1)];
	int bpp    = img->bits_per_pixel / 8;
	int stride = img->bytes_per_line;

	for (int i = 0; i < num_rectangles; i++) {
		XRectangle r = lock->rectangles[i];
		for (int y = 0; y < r.height; y++) {
			uint8_t *row = (uint8_t *)img->data + (r.y + y) * stride + r.x * bpp;
			for (int x = 0; x < r.width; x++) {
				uint8_t *px = row + x * bpp;
				/* Assume 24/32 bpp TrueColor */
				px[0] = fg & 0xFF;        /* B */
				px[1] = (fg >> 8) & 0xFF; /* G */
				px[2] = (fg >> 16) & 0xFF;/* R */
				if (bpp == 4)
					px[3] = 0xFF; /* alpha/unused */
			}
		}
	}
}
