/*
 * This file contains patch control flags.
 *
 * In principle you should be able to mix and match any patches
 * you may want. In cases where patches are logically incompatible
 * one patch may take precedence over the other as noted in the
 * relevant descriptions.
 */

/* Patches */

/* This patch allows for a command to be executed after a specified time of inactivity.
 * https://tools.suckless.org/slock/patches/auto-timeout/
 */
#define AUTO_TIMEOUT_PATCH 0

/* This patch sets the lockscreen picture to a blured or pixelated screenshot.
 * This patch depends on the Imlib2 library, uncomment the relevant line in
 * config.mk when enabling this patch.
 * The background image patch takes precedence over this patch.
 * https://tools.suckless.org/slock/patches/blur-pixelated-screen/
 */
#define BLUR_PIXELATED_SCREEN_PATCH 0

/* This patch draws the dwm logo which changes color based on the state.
 * https://tools.suckless.org/slock/patches/dwmlogo/
 */
#define DWM_LOGO_PATCH 0

/* This patch allows for commands to be executed when the user enters special passwords.
 * https://tools.suckless.org/slock/patches/secret-password/
 */
#define SECRET_PASSWORD_PATCH 0

/* This patch keeps the screen unlocked but keeps the input locked. That is, the screen
 * is not affected by slock, but users will not be able to interact with the X session
 * unless they enter the correct password.
 * https://tools.suckless.org/slock/patches/unlock_screen/
 */
#define UNLOCKSCREEN_PATCH 0
