static void
resizerectangles(struct lock *lock)
{
	int i;

	if (!lock->rectangles) {
		lock->rectangles = calloc(num_rectangles, sizeof(XRectangle));
	}

	for (i = 0; i < num_rectangles; i++){
		lock->rectangles[i].x = (rectangles[i].x * logosize)
								+ lock->xoff + ((lock->mw) / 2) - (logow / 2 * logosize);
		lock->rectangles[i].y = (rectangles[i].y * logosize)
								+ lock->yoff + ((lock->mh) / 2) - (logoh / 2 * logosize);
		lock->rectangles[i].width = rectangles[i].width * logosize;
		lock->rectangles[i].height = rectangles[i].height * logosize;
	}
}

static void
drawlogo(Display *dpy, struct lock *lock, int color)
{
	if (enabled(BackgroundImage)) {
		lock->drawable = lock->bgmap;
	} else {
		XSetForeground(dpy, lock->gc, lock->colors[BACKGROUND]);
		XFillRectangle(dpy, lock->drawable, lock->gc, 0, 0, lock->x, lock->y);
	}
	XSetForeground(dpy, lock->gc, lock->colors[color]);
	XFillRectangles(dpy, lock->drawable, lock->gc, lock->rectangles, num_rectangles);
	XCopyArea(dpy, lock->drawable, lock->win, lock->gc, 0, 0, lock->x, lock->y, 0, 0);
	XSync(dpy, False);
}
