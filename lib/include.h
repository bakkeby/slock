/* Patches */
#include "func.h"

#if HAVE_IMLIB
#include "background_image.h"
#endif

#include "dwmlogo.h"
#include "secret_commands.h"

#if HAVE_PAM
#include "pamauth.h"
#endif

#if HAVE_PTHREAD
#include "auto_timeout.h"
#endif

#include "keypress_feedback.h"
