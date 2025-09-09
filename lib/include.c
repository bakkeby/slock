/* Patches */
#include "filters/filters.c"
#include "conf.c"
#include "func.c"
#include "background_image.c"
#include "logo.c"
#include "keypress_feedback.c"
#include "secret_commands.c"
#include "xresources.c"

#if HAVE_PAM
#include "pamauth.c"
#endif

#if HAVE_PTHREAD
#include "auto_timeout.c"
#endif
