/* user and group to drop privileges to */
static const char *def_user  = "nobody";
static const char *def_group = "nogroup"; // use "nobody" for arch

static char *def_colorname[NUMCOLS] = {
	[BACKGROUND] = "#2d2d2d", /* used in the context of logo */
	[INIT] = "black",         /* after initialization */
	[INPUT] = "#005577",      /* during input */
	[FAILED] = "#CC3333",     /* wrong password */
	[CAPS] = "red",           /* CapsLock on */
	[PAM] = "#9400D3",        /* waiting for PAM */
	[BLOCKS] = "#ffffff",     /* key feedback block */
};

/* treat a cleared input like a wrong password (color) */
static int failonclear = 1;
