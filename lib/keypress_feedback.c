static void
draw_key_feedback(Display *dpy, struct lock **locks, int screen)
{
	XGCValues gr_values;
	int x, y, x_position, y_position;
	int width, height, screen_width, screen_height;
	double block_width, block_height;
	unsigned int sw, sh, du;

	Window win = locks[screen]->win;
	Window root_win;

	gr_values.foreground = locks[screen]->colors[BLOCKS];
	GC gc = XCreateGC(dpy, win, GCForeground, &gr_values);

	XGetGeometry(dpy, win, &root_win, &x, &y, &sw, &sh, &du, &du);

	static VHSState vhs = {0};
	effect_vhs_pixmap(dpy, locks[screen]->bgmap, gc, &vhs);
	XSetWindowBackgroundPixmap(dpy, locks[screen]->win, locks[screen]->bgmap);

	screen_width = (int)sw - blocks_x_min;
	screen_height = (int)sh - blocks_y_min;

	if (blocks_x_max && screen_width > blocks_x_max - blocks_x_min) {
		screen_width = blocks_x_max - blocks_x_min;
	}

	if (blocks_y_max && screen_height > blocks_y_max - blocks_y_min) {
		screen_height = blocks_y_max - blocks_y_min;
	}

	width = blocks_width ? blocks_width : screen_width / blocks_x_count;
	height = blocks_height ? blocks_height : screen_height / blocks_y_count;

	block_width = screen_width / blocks_x_count;
	block_height = screen_height / blocks_y_count;
	x_position = rand() % blocks_x_count;
	y_position = rand() % blocks_y_count;

	x = blocks_x_min + x_position*block_width;
	y = blocks_y_min + y_position*block_height;

	XFillRectangle(dpy, win, gc, x, y, width, height);

	XFreeGC(dpy, gc);
	XClearWindow(dpy, locks[screen]->win);
	XFlush(dpy);
}
