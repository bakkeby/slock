void
filter_wallpaper(XImage *img, EffectParams *p, struct lock *lock)
{
	Monitor *m;
	int idx;

	if (!p->num_string_parameters)
		return;

	float blend = (float)(p->parameters[0] ? p->parameters[0] : 1.0);
	blend = CLAMP(blend, 0.0, 1.0);

	for (m = lock->m, idx = 0; m; m = m->next, idx++) {
		load_image_from_string(lock->dpy, m, p->string_parameters[idx % p->num_string_parameters], blend);
	}
}

void
filter_image(XImage *img, EffectParams *p, struct lock *lock)
{
	Monitor *m;
	int idx;

	if (!p->num_string_parameters)
		return;

	int target_monitor = p->parameters[0];
	float blend = (float)(p->parameters[1] ? p->parameters[1] : 1.0);
	blend = CLAMP(blend, 0.0, 1.0);

	for (m = lock->m, idx = 0; m; m = m->next, idx++) {
		if (idx != target_monitor)
			continue;

		load_image_from_string(lock->dpy, m, p->string_parameters[0], blend);
	}
}
