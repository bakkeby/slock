/* user and group to drop privileges to */
static const char *def_user  = "nobody";
static const char *def_group = "nogroup"; // use "nobody" for arch

static char *def_colorname[NUMCOLS] = {
	#if DWM_LOGO_PATCH && !BLUR_PIXELATED_SCREEN_PATCH
	[BACKGROUND] =  "#2d2d2d", /* after initialization */
	#endif // DWM_LOGO_PATCH
	[INIT] =   "black",     /* after initialization */
	[INPUT] =  "#005577",   /* during input */
	[FAILED] = "#CC3333",   /* wrong password */
	[CAPS] =   "red",       /* CapsLock on */
	[PAM] =    "#9400D3",   /* waiting for PAM */
	[BLOCKS] = "#ffffff",   /* key feedback block */
};

#if DWM_LOGO_PATCH
/* insert grid pattern with scale 1:1, the size can be changed with logosize */
static const int logosize = 75;
static const int logow = 12;   /* grid width and height for right center alignment*/
static const int logoh = 6;

static XRectangle rectangles[] = {
   /* x    y   w   h */
   { 0,    3,  1,  3 },
   { 1,    3,  2,  1 },
   { 0,    5,  8,  1 },
   { 3,    0,  1,  5 },
   { 5,    3,  1,  2 },
   { 7,    3,  1,  2 },
   { 8,    3,  4,  1 },
   { 9,    4,  1,  2 },
   { 11,   4,  1,  2 },
};
#endif // DWM_LOGO_PATCH

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

#if SECRET_PASSWORD_PATCH
static const secretpass scom[] = {
	/* Password             command */
	{ "shutdown",           "doas poweroff"},
};
#endif // SECRET_PASSWORD_PATCH

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
