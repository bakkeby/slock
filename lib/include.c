/* Patches */
#include "func.c"

#if HAVE_IMLIB
#include "background_image.c"
#endif

#if BLUR_PIXELATED_SCREEN_PATCH
#include "blur_pixelated_screen.c"
#endif

#if DWM_LOGO_PATCH
#include "dwmlogo.c"
#endif

#include "keypress_feedback.c"

#if HAVE_PAM
#include "pamauth.c"
#endif

#include "xresources.c"
