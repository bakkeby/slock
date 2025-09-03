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
	#if PAMAUTH_PATCH
	[PAM] =    "#9400D3",   /* waiting for PAM */
	#endif // PAMAUTH_PATCH
	[BLOCKS] = "#ffffff",   /* key feedback block */
};

#if MESSAGE_PATCH || COLOR_MESSAGE_PATCH
/* default message */
static const char * message = "Suckless: Software that sucks less.";

/* text color */
static const char * text_color = "#ffffff";

/* text size (must be a valid size) */
static const char * font_name = "6x10";
#endif // MESSAGE_PATCH | COLOR_MESSAGE_PATCH

#if BACKGROUND_IMAGE_PATCH
/* Background image path, should be available to the user above */
static const char * background_image = "";
#endif // BACKGROUND_IMAGE_PATCH

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

#if BLUR_PIXELATED_SCREEN_PATCH
/* Enable blur */
#define BLUR
/* Set blur radius */
static const int blurRadius = 5;
/* Enable Pixelation */
//#define PIXELATION
/* Set pixelation radius */
static const int pixelSize = 10;
#endif // BLUR_PIXELATED_SCREEN_PATCH

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



#if PAMAUTH_PATCH
/* PAM service that's used for authentication */
static const char* pam_service = "login";
#endif // PAMAUTH_PATCH

/* time in seconds to cancel lock with mouse movement or keypress */
static int timetocancel = 0;
