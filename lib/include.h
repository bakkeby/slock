/* Patches */
#include "filters/filters.h"
#include "func.h"
#include "background_image.h"
#include "logo.h"
#include "secret_commands.h"
#include "keypress_feedback.h"

#if HAVE_PAM
#include "pamauth.h"
#endif

#if HAVE_PTHREAD
#include "auto_timeout.h"
#endif
