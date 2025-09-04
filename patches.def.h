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

/* This patch allows for commands to be executed when the user enters special passwords.
 * https://tools.suckless.org/slock/patches/secret-password/
 */
#define SECRET_PASSWORD_PATCH 0
