#include <Imlib2.h>

static char *expandhome(const char *string);
static void create_lock_image(Display *dpy);
static void render_lock_image(Display *dpy, struct lock *lock, Imlib_Image image);