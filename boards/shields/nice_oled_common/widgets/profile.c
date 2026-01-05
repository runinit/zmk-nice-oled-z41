#include <zephyr/kernel.h>
#include "profile.h"
#if !IS_ENABLED(CONFIG_NICE_EPAPER_ON)
// use custom_fonts.h only for the draw_active_profile_text function
#include <fonts.h>
#include <stdio.h>
#endif // !IS_ENABLED(CONFIG_NICE_EPAPER_ON)

LV_IMG_DECLARE(profiles);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG)
#define OFFSET_X 0
#define OFFSET_Y 129

LV_IMG_DECLARE(profile);
LV_IMG_DECLARE(profile_active);
#endif

#if !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG)
static void draw_inactive_profiles(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &profiles;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X,
        .y1 = CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y,
        .x2 = CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X + profiles.header.w - 1,
        .y2 = CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y + profiles.header.h - 1
    };
    lv_draw_image(&layer, &img_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

static void draw_active_profile(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    int offset = state->active_profile_index * 7;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X + offset,
        .y1 = CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y,
        .x2 = CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_X + offset + 2,
        .y2 = CONFIG_NICE_OLED_WIDGET_PROFILE_CUSTOM_Y + 2
    };
    lv_draw_rect(&layer, &rect_white_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}
#endif // !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG)

#if !IS_ENABLED(CONFIG_NICE_EPAPER_ON)
static void draw_active_profile_text(lv_obj_t *canvas, const struct status_state *state) {
    // new label_dsc
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_8, LV_TEXT_ALIGN_LEFT);

    char text[14] = {};
    snprintf(text, sizeof(text), "%d", state->active_profile_index + 1);
    label_dsc.text = text;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_X,
        .y1 = CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_Y,
        .x2 = CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_X + 34,
        .y2 = CONFIG_NICE_OLED_WIDGET_PROFILE_TEXT_CUSTOM_Y + 7
    };
    lv_draw_label(&layer, &label_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}
#endif // CONFIG_NICE_EPAPER_ON

void draw_profile_status(lv_obj_t *canvas, const struct status_state *state) {
#if !IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    draw_active_profile_text(canvas, state);
#endif // CONFIG_NICE_EPAPER_ON

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_PROFILE_BIG) //  && IS_ENABLED(CONFIG_NICE_EPAPER_ON)
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    for (int i = 0; i < 5; i++) {
        const lv_image_dsc_t *img = (i == state->active_profile_index) ? &profile_active : &profile;
        img_dsc.src = img;
        lv_area_t area = {
            .x1 = OFFSET_X + (i * 14),
            .y1 = OFFSET_Y,
            .x2 = OFFSET_X + (i * 14) + img->header.w - 1,
            .y2 = OFFSET_Y + img->header.h - 1
        };
        lv_draw_image(&layer, &img_dsc, &area);
    }
    lv_canvas_finish_layer(canvas, &layer);
#else
    draw_inactive_profiles(canvas, state);
    draw_active_profile(canvas, state);
#endif
}
