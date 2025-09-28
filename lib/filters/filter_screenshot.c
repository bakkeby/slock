void
filter_screenshot(XImage *img, EffectParams *p, struct lock *lock)
{
    fprintf(stderr, "yay, doing filter screenshot TODO\n");
    XImage *rootimg = XGetImage(lock->dpy, lock->root, 0, 0,
                                lock->x, lock->y, AllPlanes, ZPixmap);
    memcpy(img->data, rootimg->data,
           img->bytes_per_line * img->height);
    XDestroyImage(rootimg);
}
