#include <libconfig.h>

#define DIR_MAX 4080
#define PATH_MAX 4096
const char *progname = "slock";

static char *user = NULL;
static char *group = NULL;
static char **colorname = NULL;
static ResourcePref *resources = NULL;
static int num_resources = 0;

static void set_config_path(const char* filename, char *config_path, char *config_file);

static int config_lookup_strdup(const config_t *cfg, const char *name, char **strptr);
static int config_setting_lookup_strdup(const config_setting_t *cfg, const char *name, char **strptr);
static int _config_setting_strdup_string(const config_setting_t *cfg_item, char **strptr);

static int config_lookup_sloppy_bool(const config_t *cfg, const char *name, int *ptr);
static int config_setting_lookup_sloppy_bool(const config_setting_t *cfg, const char *name, int *ptr);
static int _config_setting_get_sloppy_bool(const config_setting_t *cfg, int *ptr);

static int config_lookup_simple_float(const config_t *cfg, const char *name, float *floatptr);
static int config_setting_lookup_simple_float(const config_setting_t *cfg, const char *name, float *floatptr);
static int _config_setting_get_simple_float(const config_setting_t *cfg_item, float *floatptr);

static void cleanup_config(void);
static void load_config(void);
static void load_fallback_config(void);
static void load_misc(config_t *cfg);
static void load_colors(config_t *cfg);

static void generate_resource_strings(void);
static void add_resource_binding(const char *string, void *ptr);

void
set_config_path(const char* filename, char *config_path, char *config_file)
{
	const char *xdg_config_home = getenv("XDG_CONFIG_HOME");
	const char *home = getenv("HOME");

	if (xdg_config_home && xdg_config_home[0] != '\0') {
		snprintf(config_path, DIR_MAX, "%s/%s/", xdg_config_home, progname);
	} else if (home) {
		snprintf(config_path, DIR_MAX, "%s/.config/%s/", home, progname);
	} else {
		return;
	}

	snprintf(config_file, PATH_MAX, "%s/%s.cfg", config_path, filename);
}

int
config_lookup_strdup(const config_t *cfg, const char *name, char **strptr)
{
	return _config_setting_strdup_string(config_lookup(cfg, name), strptr);
}


int
config_setting_lookup_strdup(const config_setting_t *cfg, const char *name, char **strptr)
{
	return _config_setting_strdup_string(config_setting_lookup(cfg, name), strptr);
}

int
_config_setting_strdup_string(const config_setting_t *cfg_item, char **strptr)
{
	if (!cfg_item)
		return 0;

	const char *string = config_setting_get_string(cfg_item);

	if (!string)
		return 0;

	free(*strptr);
	*strptr = strdup(string);
	return 1;
}

int
config_lookup_sloppy_bool(const config_t *cfg, const char *name, int *ptr)
{
	return _config_setting_get_sloppy_bool(config_lookup(cfg, name), ptr);
}

int
config_setting_lookup_sloppy_bool(const config_setting_t *cfg, const char *name, int *ptr)
{
	return _config_setting_get_sloppy_bool(config_setting_lookup(cfg, name), ptr);
}

int
_config_setting_get_sloppy_bool(const config_setting_t *cfg_item, int *ptr)
{
	const char *string;

	if (!cfg_item)
		return 0;

	switch (config_setting_type(cfg_item)) {
	case CONFIG_TYPE_INT:
		*ptr = config_setting_get_int(cfg_item);
		return 1;
	case CONFIG_TYPE_STRING:
		string = config_setting_get_string(cfg_item);

		if (string && strlen(string)) {
			char a = tolower(string[0]);
			/* Match for positives like "true", "yes" and "on" */
			*ptr = (a == 't' || a == 'y' || !strcasecmp(string, "on"));
			return 1;
		}
		break;
	case CONFIG_TYPE_BOOL:
		*ptr = config_setting_get_bool(cfg_item);
		return 1;
	}

	return 0;
}

int
config_lookup_simple_float(const config_t *cfg, const char *name, float *floatptr)
{
	return _config_setting_get_simple_float(config_lookup(cfg, name), floatptr);
}

int
config_setting_lookup_simple_float(const config_setting_t *cfg, const char *name, float *floatptr)
{
	return _config_setting_get_simple_float(config_setting_lookup(cfg, name), floatptr);
}

int
_config_setting_get_simple_float(const config_setting_t *cfg_item, float *floatptr)
{
	if (!cfg_item)
		return 0;

	double value = config_setting_get_float(cfg_item);

	*floatptr = (float)value;
	return 1;
}

void
load_config(void)
{
	config_t cfg;
	char config_path[DIR_MAX] = {0};
	char config_file[PATH_MAX] = {0};

	set_config_path(progname, config_path, config_file);
	config_init(&cfg);
	config_set_include_dir(&cfg, config_path);

	if (config_read_file(&cfg, config_file)) {
		// load_functionality(&cfg);
		load_misc(&cfg);
		load_colors(&cfg);
		// load_commands(&cfg);
		// load_fonts(&cfg);
		// load_keybindings(&cfg);
		// load_mouse_bindings(&cfg);
		// load_mouse_cursor(&cfg);
		// load_window_icon(&cfg);
	} else if (strcmp(config_error_text(&cfg), "file I/O error")) {
		fprintf(stderr, "Error reading config at %s\n", config_file);
		fprintf(stderr, "%s:%d - %s\n",
			config_error_file(&cfg),
			config_error_line(&cfg),
			config_error_text(&cfg)
		);
	}

	load_fallback_config();
	generate_resource_strings();
	config_destroy(&cfg);
}

void
load_fallback_config(void)
{
	if (!user) {
		user = strdup(def_user);
	}

	if (!group) {
		group = strdup(def_group);
	}

	if (!colorname) {
		colorname = def_colorname;
	}
}

void
cleanup_config(void)
{
	int i;

	free(user);
	free(group);

	if (colorname != def_colorname) {
		for (i = 0; i < NUMCOLS; i++) {
			free(colorname[i]);
		}
		free(colorname);
	}

	for (i = 0; i < num_resources; i++)
		free(resources[i].name);
	free(resources);
}

void
load_misc(config_t *cfg)
{
	config_lookup_strdup(cfg, "user", &user);
	config_lookup_strdup(cfg, "group", &group);
	config_lookup_sloppy_bool(cfg, "fail_on_clear", &failonclear);
	config_lookup_float(cfg, "alpha", &alpha);
}

void
load_colors(config_t *cfg)
{
	config_setting_t *cols;

	cols = config_lookup(cfg, "colors");
	if (!cols || !config_setting_is_group(cols))
		return;

	colorname = calloc(NUMCOLS, sizeof(char *));

	#if DWM_LOGO_PATCH && !BLUR_PIXELATED_SCREEN_PATCH
	config_setting_lookup_strdup(cols, "background", &colorname[BACKGROUND]);
	#endif
	config_setting_lookup_strdup(cols, "init", &colorname[INIT]);
	config_setting_lookup_strdup(cols, "input", &colorname[INPUT]);
	config_setting_lookup_strdup(cols, "failed", &colorname[FAILED]);

	#if CAPSCOLOR_PATCH
	config_setting_lookup_strdup(cols, "caps", &colorname[CAPS]);
	#endif
	#if PAMAUTH_PATCH
	config_setting_lookup_strdup(cols, "pam", &colorname[PAM]);
	#endif // PAMAUTH_PATCH
	#if KEYPRESS_FEEDBACK_PATCH
	config_setting_lookup_strdup(cols, "blocks", &colorname[BLOCKS]);
	#endif
}

void
generate_resource_strings(void)
{
	resources = calloc(NUMCOLS + 10, sizeof(ResourcePref));

	/* Add resource strings */
	#if DWM_LOGO_PATCH && !BLUR_PIXELATED_SCREEN_PATCH
	add_resource_binding("background", &colorname[BACKGROUND]);
	#endif

	add_resource_binding("locked", &colorname[INIT]);
	add_resource_binding("input", &colorname[INPUT]);
	add_resource_binding("failed", &colorname[FAILED]);

	#if CAPSCOLOR_PATCH
	add_resource_binding("capslock", &colorname[CAPS]);
	#endif

	#if PAMAUTH_PATCH
	add_resource_binding("pamauth", &colorname[PAM]);
	#endif

	#if MESSAGE_PATCH || COLOR_MESSAGE_PATCH
	add_resource_binding("message", &message);
	add_resource_binding("text_color", &text_color);
	add_resource_binding("font_name", &font_name);
	#endif

	#if BACKGROUND_IMAGE_PATCH
	add_resource_binding("bg_image", &def_background_image);
	#endif
}

void
add_resource_binding(const char *string, void *ptr)
{
	resources[num_resources].name = strdup(string);
	resources[num_resources].type = STRING;
	resources[num_resources].dst = ptr;
	num_resources++;
}

