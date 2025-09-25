#include <math.h>

typedef void (FilterFunc)(XImage *img, double parameters[8], struct lock *lock);

static void filter_bloom(XImage *img, double parameters[8], struct lock *lock);
static void filter_blur(XImage *img, double parameters[8], struct lock *lock);
static void filter_channels(XImage *img, double parameters[8], struct lock *lock);
static void filter_chroma_crawl(XImage *img, double parameters[8], struct lock *lock);
static void filter_chroma_drift(XImage *img, double parameters[8], struct lock *lock);
static void filter_color_bleed(XImage *img, double parameters[8], struct lock *lock);
static void filter_crt_effect(XImage *img, double parameters[8], struct lock *lock);
static void filter_dual_kawase_blur(XImage *img, double parameters[8], struct lock *lock);
static void filter_film_grain(XImage *img, double parameters[8], struct lock *lock);
static void filter_ghosting(XImage *img, double parameters[8], struct lock *lock);
static void filter_grayscale(XImage *img, double parameters[8], struct lock *lock);
static void filter_horizontal_tear(XImage *img, double parameters[8], struct lock *lock);
static void filter_invert(XImage *img, double parameters[8], struct lock *lock);
static void filter_logo(XImage *img, double parameters[8], struct lock *lock);
static void filter_noise(XImage *img, double parameters[8], struct lock *lock);
static void filter_soft_noise(XImage *img, double parameters[8], struct lock *lock);
static void filter_modulation_glitch(XImage *img, double parameters[8], struct lock *lock);
static void filter_mosaic(XImage *img, double parameters[8], struct lock *lock);
static void filter_pixelate(XImage *img, double parameters[8], struct lock *lock);
static void filter_posterize(XImage *img, double parameters[8], struct lock *lock);
static void filter_random_blocks(XImage *img, double parameters[8], struct lock *lock);
static void filter_scanlines(XImage *img, double parameters[8], struct lock *lock);
static void filter_sobel(XImage *img, double parameters[8], struct lock *lock);
static void filter_solid_color(XImage *img, double parameters[8], struct lock *lock);
static void filter_temperature(XImage *img, double parameters[8], struct lock *lock);
static void filter_rain_shift_vert(XImage *img, double parameters[8], struct lock *lock);
static void filter_rain_shift_horz(XImage *img, double parameters[8], struct lock *lock);
static void filter_vignette(XImage *img, double parameters[8], struct lock *lock);
static void filter_vhs_jitter(XImage *img, double parameters[8], struct lock *lock);
static void filter_vhs_warp_chroma(XImage *img, double parameters[8], struct lock *lock);
static void filter_wave_distortion(XImage *img, double parameters[8], struct lock *lock);

static inline uint8_t clamp255(int v) {
	return v < 0 ? 0 : (v > 255 ? 255 : v);
}

struct effect_map {
	const char *name;
	FilterFunc *func;
};

typedef struct EffectFilter EffectFilter;
struct EffectFilter {
	FilterFunc *func;
	double parameters[8];
};

static const struct effect_map effect_names[] = {
	{ "bloom", filter_bloom },
	{ "blur", filter_blur },
	{ "channels", filter_channels },
	{ "chroma_crawl", filter_chroma_crawl },
	{ "chroma_drift", filter_chroma_drift },
	{ "color_bleed", filter_color_bleed },
	{ "crt_effect", filter_crt_effect },
	{ "dual_kawase_blur", filter_blur },
	{ "film_grain", filter_film_grain },
	{ "ghosting", filter_ghosting },
	{ "grayscale", filter_grayscale },
	{ "horizontal_tear", filter_horizontal_tear },
	{ "invert", filter_invert },
	{ "logo", filter_logo },
	{ "modulation_glitch", filter_modulation_glitch },
	{ "mosaic", filter_mosaic },
	{ "noise", filter_noise },
	{ "soft_noise", filter_soft_noise },
	{ "pixelate", filter_pixelate },
	{ "posterize", filter_posterize },
	{ "random_blocks", filter_random_blocks },
	{ "rain_shift_vert", filter_rain_shift_vert },
	{ "rain_shift_horz", filter_rain_shift_horz },
	{ "scanlines", filter_scanlines },
	{ "sobel", filter_sobel },
	{ "solid_color", filter_solid_color },
	{ "temperature", filter_temperature },
	{ "vignette", filter_vignette },
	{ "vhs_jitter", filter_vhs_jitter },
	{ "vhs_warp_chroma", filter_vhs_warp_chroma },
	{ "wave_distortion", filter_wave_distortion },
	{ NULL, 0 }
};
