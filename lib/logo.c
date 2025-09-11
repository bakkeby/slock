static void
resizerectangles(struct lock *lock)
{
	int i;

	if (!rectangles)
		return;

	if (!lock->rectangles) {
		lock->rectangles = calloc(num_rectangles, sizeof(XRectangle));
	}

	for (i = 0; i < num_rectangles; i++){
		lock->rectangles[i].x = lock->m->mx + (rectangles[i].x * logosize)
								+ ((lock->m->mw) / 2) - ((float)logow / 2 * logosize);
		lock->rectangles[i].y = lock->m->my + (rectangles[i].y * logosize)
								+  ((lock->m->mh) / 2) - ((float)logoh / 2 * logosize);
		lock->rectangles[i].width = rectangles[i].width * logosize;
		lock->rectangles[i].height = rectangles[i].height * logosize;
	}
}

static void
drawlogo(Display *dpy, struct lock *lock, int fg)
{
	double parameters[8] = { fg };
	filter_logo(image, parameters, lock);

	/* Push image into pixmap and set window background */
	XShmPutImage(dpy, lock->bgmap, lock->gc, image,
             0, 0, 0, 0, lock->x, lock->y, False);
	XSetWindowBackgroundPixmap(dpy, lock->win, lock->bgmap);
	XClearWindow(dpy, lock->win);
	XFlush(dpy);
	XSync(dpy, False);
}
