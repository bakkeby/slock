static void
draw_key_feedback(Display *dpy, struct lock **locks, int screen)
{
	int i;
	struct lock *lock = locks[screen];

	if (!num_feedback_filters)
		return;

	for (i = 0; i < num_feedback_filters; i++) {
		if (feedback_filters[i].func) {
			feedback_filters[i].func(image, feedback_filters[i].parameters, lock);
		}
	}

	XShmPutImage(dpy, lock->bgmap, lock->gc, image, 0, 0, 0, 0, lock->x, lock->y, False);
	XClearWindow(dpy, lock->win);
	XFlush(dpy);
}
