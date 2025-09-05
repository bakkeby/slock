static void
resizerectangles(struct lock *lock)
{
	int i;

	if (!lock->rectangles) {
		lock->rectangles = calloc(num_rectangles, sizeof(XRectangle));
	}

	for (i = 0; i < num_rectangles; i++){
		lock->rectangles[i].x = lock->xoff + (rectangles[i].x * logosize)
								+ ((lock->mw) / 2) - ((float)logow / 2 * logosize);
		lock->rectangles[i].y = lock->yoff + (rectangles[i].y * logosize)
								+  ((lock->mh) / 2) - ((float)logoh / 2 * logosize);
		lock->rectangles[i].width = rectangles[i].width * logosize;
		lock->rectangles[i].height = rectangles[i].height * logosize;
	}
}

static void
drawlogo(Display *dpy, struct lock *lock, int color)
{
	#if HAVE_IMLIB
	if (enabled(BackgroundImage)) {
		// lock->drawable = lock->bgmap;
	} else {
		XSetForeground(dpy, lock->gc, lock->colors[BACKGROUND]);
		XFillRectangle(dpy, lock->drawable, lock->gc, 0, 0, lock->x, lock->y);
	}
	#else
	XSetForeground(dpy, lock->gc, lock->colors[BACKGROUND]);
	XFillRectangle(dpy, lock->drawable, lock->gc, 0, 0, lock->x, lock->y);
	#endif
	XSetForeground(dpy, lock->gc, lock->colors[color]);
	XFillRectangles(dpy, lock->drawable, lock->gc, lock->rectangles, num_rectangles);
	XCopyArea(dpy, lock->drawable, lock->win, lock->gc, 0, 0, lock->x, lock->y, 0, 0);
	XSync(dpy, False);
}
