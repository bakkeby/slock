/* Patches */
#include "func.c"

#if HAVE_IMLIB
#include "background_image.c"
#endif

#include "dwmlogo.c"
#include "keypress_feedback.c"
#include "secret_commands.c"

#if HAVE_PAM
#include "pamauth.c"
#endif

#if HAVE_PTHREAD
#include "auto_timeout.c"
#endif

#include "xresources.c"
