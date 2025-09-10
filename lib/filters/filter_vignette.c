/* Rounded-rectangle style vignette for any aspect ratio */
void
filter_vignette(XImage *img, double parameters[8], struct lock *lock)
{
    if (!img || !img->data || !lock) return;

    int bpp = (img->bits_per_pixel + 7) / 8;
    if (bpp < 3) return;
    int stride = img->bytes_per_line;

    double strength    = (parameters[0] != 0.0) ? parameters[0] : 0.5; // overall vignette strength
    double edge_frac_x = (parameters[1] != 0.0) ? parameters[1] : 0.1; // horizontal edge fraction
    double edge_frac_y = (parameters[2] != 0.0) ? parameters[2] : 0.1; // vertical edge fraction
    double corner_r    = (parameters[3] != 0.0) ? parameters[3] : 50.0; // corner radius
    double falloff     = (parameters[4] != 0.0) ? parameters[4] : 2.0;  // smoothness exponent

    int xoff = lock->xoff;
    int yoff = lock->yoff;
    int mw   = lock->mw;
    int mh   = lock->mh;

    for (int y = 0; y < mh; y++) {
        for (int x = 0; x < mw; x++) {
            uint8_t *dst = (uint8_t*)img->data + (yoff + y) * stride + (xoff + x) * bpp;

            // normalized distances from edges (0=center, 1=edge)
            double nx = fmin((double)x / (mw * edge_frac_x),
                             (double)(mw - 1 - x) / (mw * edge_frac_x));
            double ny = fmin((double)y / (mh * edge_frac_y),
                             (double)(mh - 1 - y) / (mh * edge_frac_y));
            nx = fmin(nx, 1.0);
            ny = fmin(ny, 1.0);

            // distance from corner for rounded corners
            double dx_corner = (x < corner_r) ? corner_r - x :
                               (x >= mw - corner_r ? x - (mw - corner_r - 1) : 0);
            double dy_corner = (y < corner_r) ? corner_r - y :
                               (y >= mh - corner_r ? y - (mh - corner_r - 1) : 0);
            double dist_corner = hypot(dx_corner, dy_corner);
            double corner_mask = 1.0;
            if (dist_corner > 0) {
                if (dist_corner >= corner_r)
                    corner_mask = 0.0;
                else
                    corner_mask = (corner_r - dist_corner) / corner_r;
            }

            // combine horizontal/vertical and corner influence
            double mask = pow(fmin(nx, ny), falloff);
            mask = mask * (1.0 - strength) + strength;
            mask *= corner_mask;

            for (int c = 0; c < 3; c++)
                dst[c] = (uint8_t)(dst[c] * mask);
        }
    }
}
