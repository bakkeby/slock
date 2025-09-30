#include <libconfig.h>
#include <libgen.h>

#define DIR_MAX 4080
#define PATH_MAX 4096
const char *progname = "slock";
static char *cfg_filename = "slock.cfg";

static char *user = NULL;
static char *group = NULL;
static char **colorname = NULL;
static EffectParams *background_filters = NULL;
static EffectParams *feedback_filters = NULL;
static EffectParams *failure_filters = NULL;
static ResourcePref *resources = NULL;
uint64_t settings = 0;
static int num_resources = 0;
static int num_secret_commands = 0;
static int num_background_filters = 0;
static int num_feedback_filters = 0;
static int num_failure_filters = 0;
static char *failure_command = NULL;
static int failure_command_run_once = 0;
static int failure_count = 0;
static int dpms_timeout = 0;
static int logosize = 75;
static int logow = 12;
static int logoh = 6;
static XRectangle *rectangles = NULL;
static secretpass *secret_commands = NULL;
static int auto_timeout_run_once = 0;
static int auto_timeout_offset = 0;
static char *auto_timeout_command = NULL;
static char *exit_command = NULL;
static int timetocancel = 0;
static double alpha = 1.0;

/* PAM service that's used for authentication */
static char *pam_service = NULL;

#define map(F) { #F, F }

static const struct nv float_string_names[] = {
	map(INIT),
	map(INPUT),
	map(FAILED),
	map(CAPS),
	map(PAM),
	map(BLOCKS),
	{ NULL, 0 }
};

#undef map

#include "lib/libconfig_helper_functions.c"

static void set_config_path(const char* filename, char *config_path, char *config_file);

static void cleanup_config(void);
static void load_config(void);
static void load_auto_timeout(config_t *cfg);
static void load_fallback_config(void);
static void load_misc(config_t *cfg);
static void load_background(config_t *cfg);
static void load_colors(config_t *cfg);
static void load_functionality(config_t *cfg);
static void load_keypress_feedback(config_t *cfg);
static void load_logo(config_t *cfg);
static void load_on_failure(config_t *cfg);
static void load_secret_commands(config_t *cfg);
static void load_filters(config_setting_t *filters_t, int *num_filters, EffectParams **filter_params);

static FilterFunc *parse_effect_filter(const char *name);
static void generate_resource_strings(void);
static void add_resource_binding(const char *string, void *ptr);

int startswith(const char *needle, const char *haystack)
{
	return !strncmp(haystack, needle, strlen(needle));
}

void
set_config_path(const char* filename, char *config_path, char *config_file)
{
    const char *xdg_config_home = getenv("XDG_CONFIG_HOME");
    const char *home = getenv("HOME");

    if (startswith("/", filename)) {
        char *dname = strdup(filename);
        snprintf(config_path, PATH_MAX, "%s", dirname(dname));
        snprintf(config_file, PATH_MAX, "%s", filename);
        free(dname);
        return;
    }

    if (xdg_config_home && xdg_config_home[0] != '\0') {
        snprintf(config_path, PATH_MAX, "%s/%s", xdg_config_home, progname);
        snprintf(config_file, PATH_MAX, "%s/%s", config_path, filename);
    } else if (home) {
        snprintf(config_path, PATH_MAX, "%s/.config/%s", home, progname);
        snprintf(config_file, PATH_MAX, "%s/%s", config_path, filename);
    }
}

void
load_config(void)
{
	config_t cfg;
	char config_path[DIR_MAX] = {0};
	char config_file[PATH_MAX] = {0};

	const char *envcfg = getenv("SLOCK_CONFIG_PATH");
	const char *filename = (envcfg && strlen(envcfg) ? envcfg : cfg_filename);

	set_config_path(filename, config_path, config_file);
	config_init(&cfg);
	config_set_include_dir(&cfg, config_path);

	if (config_read_file(&cfg, config_file)) {
		load_functionality(&cfg);
		load_misc(&cfg);
		load_background(&cfg);
		load_colors(&cfg);
		load_logo(&cfg);
		load_on_failure(&cfg);
		load_keypress_feedback(&cfg);
		load_secret_commands(&cfg);
		load_auto_timeout(&cfg);
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

	if (!pam_service) {
		disablefunc(PAMAuthentication);
	}

	if (!auto_timeout_command) {
		disablefunc(AutoTimeout);
	}

	if (!exit_command) {
		disablefunc(ExitCommand);
	}
}

void
cleanup_config(void)
{
	int i;

	free(user);
	free(group);
	free(failure_command);
	free(exit_command);
	free(auto_timeout_command);
	free(pam_service);

	if (colorname != def_colorname) {
		for (i = 0; i < NUMCOLS; i++) {
			free(colorname[i]);
		}
		free(colorname);
	}

	for (i = 0; i < num_resources; i++)
		free(resources[i].name);
	free(resources);

	for (i = 0; i < num_secret_commands; i++) {
		free(secret_commands[i].password);
		free(secret_commands[i].command);
	}
	free(secret_commands);

}

void
load_misc(config_t *cfg)
{
	config_lookup_strdup(cfg, "user", &user);
	config_lookup_strdup(cfg, "group", &group);
	config_lookup_sloppy_bool(cfg, "fail_on_clear", &failonclear);
	config_lookup_float(cfg, "alpha", &alpha);
	config_lookup_int(cfg, "quick_cancel_timeout_seconds", &timetocancel);
	config_lookup_int(cfg, "dpms_timeout_in_seconds", &dpms_timeout);

	#if HAVE_PAM
	config_lookup_strdup(cfg, "pam_service", &pam_service);
	#endif

	config_lookup_strdup(cfg, "exit_command.run_command", &exit_command);
}

void
load_colors(config_t *cfg)
{
	config_setting_t *cols;

	cols = config_lookup(cfg, "colors");
	if (!cols || !config_setting_is_group(cols))
		return;

	colorname = calloc(NUMCOLS, sizeof(char *));

	config_setting_lookup_strdup(cols, "init", &colorname[INIT]);
	config_setting_lookup_strdup(cols, "input", &colorname[INPUT]);
	config_setting_lookup_strdup(cols, "failed", &colorname[FAILED]);
	config_setting_lookup_strdup(cols, "caps", &colorname[CAPS]);
	config_setting_lookup_strdup(cols, "pam", &colorname[PAM]);
	config_setting_lookup_strdup(cols, "blocks", &colorname[BLOCKS]);
}

void
load_functionality(config_t *cfg)
{
	int i, enabled;

	config_setting_t *func_t = config_lookup(cfg, "functionality");
	if (!func_t)
		return;

	for (i = 0; functionality_names[i].name != NULL; i++) {
		if (config_setting_lookup_sloppy_bool(func_t, functionality_names[i].name, &enabled)) {
			setenabled(functionality_names[i].value, enabled);
		}
	}
}

void
load_logo(config_t *cfg)
{
	int i;
	config_setting_t *rectangles_t, *rect_t;

	config_lookup_int(cfg, "logo.size", &logosize);

	rectangles_t = config_lookup(cfg, "logo.rectangles");
	if (!rectangles_t || !config_setting_is_list(rectangles_t))
		return;

	num_rectangles = config_setting_length(rectangles_t);
	rectangles = calloc(num_rectangles, sizeof(XRectangle));

	for (i = 0; i < num_rectangles; i++) {
		rect_t = config_setting_get_elem(rectangles_t, i);
		if (!rect_t || config_setting_length(rect_t) != 4) {
			fprintf(stderr, "logo: invalid rectangle at index %d\n", i);
			continue;
		}

		rectangles[i].x = (short) config_setting_get_int_elem(rect_t, 0);
		rectangles[i].y = (short) config_setting_get_int_elem(rect_t, 1);
		rectangles[i].width = (unsigned short) config_setting_get_int_elem(rect_t, 2);
		rectangles[i].height = (unsigned short) config_setting_get_int_elem(rect_t, 3);

		logow = MAX(logow, rectangles[i].x + rectangles[i].width);
		logoh = MAX(logoh, rectangles[i].y + rectangles[i].height);
	}
}

void
load_background(config_t *cfg)
{
	load_filters(config_lookup(cfg, "background.filters"), &num_background_filters, &background_filters);
}

void
load_keypress_feedback(config_t *cfg)
{
	load_filters(config_lookup(cfg, "keypress_feedback.filters"), &num_feedback_filters, &feedback_filters);
}

void
load_on_failure(config_t *cfg)
{
	config_lookup_int(cfg, "on_failure.after_this_many_failures", &failure_count);
	config_lookup_sloppy_bool(cfg, "on_failure.run_once", &failure_command_run_once);
	config_lookup_strdup(cfg, "on_failure.run_command", &failure_command);
	load_filters(config_lookup(cfg, "on_failure.filters"), &num_failure_filters, &failure_filters);
}

void
load_filters(config_setting_t *filters_t, int *num_filters, EffectParams **filters)
{
	int i, p, num_params;

	const char *string;
	const config_setting_t *filter_t, *params_t, *param_t;

	if (!filters_t || !config_setting_is_list(filters_t))
		return;

	*num_filters = config_setting_length(filters_t);
	*filters = calloc(*num_filters, sizeof(EffectParams));

	for (i = 0; i < *num_filters; i++) {
		filter_t = config_setting_get_elem(filters_t, i);
		if (config_setting_lookup_string(filter_t, "effect", &string)) {
			(*filters)[i].func = parse_effect_filter(string);
		}

		params_t = config_setting_lookup(filter_t, "params");
		if (!params_t)
			continue;

		num_params = setting_length(params_t);
		int str_count = 0;

		/* First pass: count strings */
		for (p = 0; p < num_params; p++) {
			param_t = config_setting_get_elem(params_t, p);
			if (config_setting_type(param_t) == CONFIG_TYPE_STRING &&
					!config_setting_parse_float_string(param_t))
				str_count++;
		}

		if (str_count > 0) {
			(*filters)[i].string_parameters = calloc(str_count, sizeof(char *));
			(*filters)[i].num_string_parameters = str_count;
		}

		/* Second pass: assign values */
		int str_index = 0;
		for (p = 0; p < num_params && p < 8; p++) {
			param_t = config_setting_get_elem(params_t, p);
			if (config_setting_type(param_t) == CONFIG_TYPE_STRING &&
					!config_setting_parse_float_string(param_t)) {
				string = config_setting_get_string(param_t);
				(*filters)[i].string_parameters[str_index++] = strdup(string);
			} else {
				(*filters)[i].parameters[p] = setting_get_float_elem(params_t, p);
			}
		}
	}
}

void
load_secret_commands(config_t *cfg)
{
	int i;
	const config_setting_t *secrets_t, *secret_t;

	secrets_t = config_lookup(cfg, "secret_commands");
	if (!secrets_t || !config_setting_is_list(secrets_t))
		return;

	num_secret_commands = config_setting_length(secrets_t);
	secret_commands = calloc(num_secret_commands, sizeof(secretpass));

	for (i = 0; i < num_secret_commands; i++) {
		secret_t = config_setting_get_elem(secrets_t, i);
		config_setting_lookup_strdup(secret_t, "password", &secret_commands[i].password);
		config_setting_lookup_strdup(secret_t, "command", &secret_commands[i].command);
	}
}

void
load_auto_timeout(config_t *cfg)
{
	config_lookup_int(cfg, "automatic_timeout.after_this_many_seconds", &auto_timeout_offset);
	config_lookup_sloppy_bool(cfg, "automatic_timeout.run_once", &auto_timeout_run_once);
	config_lookup_strdup(cfg, "automatic_timeout.run_command", &auto_timeout_command);
}

FilterFunc *
parse_effect_filter(const char *name)
{
	int i;

	if (!name)
		return NULL;

	if (startswith("filter_", name))
		name += 7;

    for (i = 0; effect_names[i].name != NULL; i++) {
        if (!strcmp(effect_names[i].name, name))
            return effect_names[i].func;
    }

    return NULL;
}

void
generate_resource_strings(void)
{
	resources = calloc(NUMCOLS, sizeof(ResourcePref));

	/* Add resource strings */
	add_resource_binding("locked", &colorname[INIT]);
	add_resource_binding("input", &colorname[INPUT]);
	add_resource_binding("failed", &colorname[FAILED]);
	add_resource_binding("capslock", &colorname[CAPS]);
	add_resource_binding("blocks", &colorname[BLOCKS]);
	add_resource_binding("pamauth", &colorname[PAM]);
}

void
add_resource_binding(const char *string, void *ptr)
{
	resources[num_resources].name = strdup(string);
	resources[num_resources].type = STRING;
	resources[num_resources].dst = ptr;
	num_resources++;
}
