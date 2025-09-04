/* user and group to drop privileges to */
static const char *def_user  = "nobody";
static const char *def_group = "nogroup"; // use "nobody" for arch

static char *def_colorname[NUMCOLS] = {
	[BACKGROUND] =  "#2d2d2d", /* after initialization */
	[INIT] =   "black",     /* after initialization */
	[INPUT] =  "#005577",   /* during input */
	[FAILED] = "#CC3333",   /* wrong password */
	[CAPS] =   "red",       /* CapsLock on */
	[PAM] =    "#9400D3",   /* waiting for PAM */
	[BLOCKS] = "#ffffff",   /* key feedback block */
};

/* lock screen opacity */
static double alpha = 0.3;

/* treat a cleared input like a wrong password (color) */
static int failonclear = 1;

#if AUTO_TIMEOUT_PATCH
/* length of time (seconds) until */
static const int timeoffset = 60;

/* should [command] be run only once? */
static const int runonce = 0;

/* command to be run after [time] has passed */
static const char *command = "doas poweroff";
#endif // AUTO_TIMEOUT_PATCH

/* Keypress feedback default settings */
static int blocks_width = 55; // 0 = full width
static int blocks_height = 0;

// Number of blocks
static int blocks_x_count = 100;
static int blocks_y_count = 100;

// position
static int blocks_y_min = 0;
static int blocks_y_max = 0;
static int blocks_x_min = 0;
static int blocks_x_max = 0;

/* time in seconds to cancel lock with mouse movement or keypress */
static int timetocancel = 0;
