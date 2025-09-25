void
filter_posterize(XImage *img, double parameters[8], struct lock *lock)
{
	int bits = (int)parameters[0];
	int affect_r = (int)parameters[1];
	int affect_g = (int)parameters[2];
	int affect_b = (int)parameters[3];

	if (!img || !img->data) return;
	if (bits < 1) bits = 1;
	if (bits > 16) bits = 16;

	Monitor *m;
	int bpp = img->bits_per_pixel / 8;
	if (bpp < 3) return; /* must have RGB */

	int stride = img->bytes_per_line;

	/* Posterization mask: keep top `bits` bits */
	int shift = 8 - bits;            /* how many LSBs to drop */
	uint8_t mask = 0xFF << shift;    /* e.g. bits=4 → 0xF0 */

	/* Iterate pixels */
	for (m = lock->m; m; m = m->next) {
		for (int y = m->my; y < m->mh; y++) {
			uint8_t *row = (uint8_t*)img->data + y * stride;
			for (int x = m->mx; x < m->mw; x++) {
				uint8_t *px = row + x * bpp;

				/* BGRX order (XImage is often little-endian) */
				if (affect_b) {
					uint8_t b = px[0] & mask;
					px[0] = b | (b >> bits);  /* replicate bits downward */
				}
				if (affect_g) {
					uint8_t g = px[1] & mask;
					px[1] = g | (g >> bits);
				}
				if (affect_r) {
					uint8_t r = px[2] & mask;
					px[2] = r | (r >> bits);
				}
			}
		}
	}
}
