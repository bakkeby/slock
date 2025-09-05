#include <Imlib2.h>

static char *expandhome(const char *string);
static void create_lock_image(Display *dpy, struct lock *lock);
static void render_lock_image(Display *dpy, struct lock *lock, XImage *image);
static int load_lock_image_from_file(Display *dpy, const char *background_image);
