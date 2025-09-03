/* Patches */
#include "func.h"

#if BACKGROUND_IMAGE_PATCH
#include "background_image.h"
#elif BLUR_PIXELATED_SCREEN_PATCH
#include "blur_pixelated_screen.h"
#endif

#if DWM_LOGO_PATCH
#include "dwmlogo.h"
#endif

#if HAVE_PAM
#include "pamauth.h"
#endif

#include "keypress_feedback.h"
