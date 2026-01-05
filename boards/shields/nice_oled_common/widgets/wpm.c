#include "wpm.h"
// #include "../assets/custom_fonts.h"
#include <math.h>
#include <zephyr/kernel.h>
// TODO: fonts global pixel_operator_mono_12
#include <fonts.h>

LV_IMG_DECLARE(gauge);
LV_IMG_DECLARE(grid);

#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_LUNA) ||                                                \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT) ||                                           \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_SPEEDOMETER)
#else
static void draw_gauge(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &gauge;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = CONFIG_NICE_OLED_WIDGET_WPM_GAUGE_CUSTOM_X,
        .y1 = CONFIG_NICE_OLED_WIDGET_WPM_GAUGE_CUSTOM_Y,
        .x2 = CONFIG_NICE_OLED_WIDGET_WPM_GAUGE_CUSTOM_X + gauge.header.w - 1,
        .y2 = CONFIG_NICE_OLED_WIDGET_WPM_GAUGE_CUSTOM_Y + gauge.header.h - 1
    };
    lv_draw_image(&layer, &img_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

static void draw_needle(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 1);

    int centerX = CONFIG_NICE_OLED_WIDGET_WPM_NEEDLE_CENTER_CUSTOM_X;
    int centerY = CONFIG_NICE_OLED_WIDGET_WPM_NEEDLE_CENTER_CUSTOM_Y;
    int offset = 13;
    int value = state->wpm[9];

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH_FIXED_RANGE)
    float max = CONFIG_NICE_OLED_WIDGET_WPM_GRAPH_FIXED_RANGE_MAX;
#else
    float max = 0;
    for (int i = 0; i < 10; i++) {
        if (state->wpm[i] > max) {
            max = state->wpm[i];
        }
    }
#endif
    if (max == 0)
        max = 100;
    if (value < 0)
        value = 0;
    if (value > max)
        value = max;

    float radius = 25.45585;
    float angleDeg = 225 + ((float)value / max) * 90;
    float angleRad = angleDeg * (3.14159 / 180.0f);

    int needleStartX = centerX + (int)(offset * cos(angleRad));
    int needleStartY = centerY + (int)(offset * sin(angleRad));
    int needleEndX = centerX + (int)(radius * cos(angleRad));
    int needleEndY = centerY + (int)(radius * sin(angleRad));

    lv_point_t points[2] = {{needleStartX, needleStartY}, {needleEndX, needleEndY}};
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    line_dsc.p1.x = points[0].x;
    line_dsc.p1.y = points[0].y;
    line_dsc.p2.x = points[1].x;
    line_dsc.p2.y = points[1].y;
    lv_draw_line(&layer, &line_dsc);
    lv_canvas_finish_layer(canvas, &layer);
}
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID) || !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH)
#else
static void draw_grid(lv_obj_t *canvas) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &grid;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = 0,
        .y1 = 65,
        .x2 = grid.header.w - 1,
        .y2 = 65 + grid.header.h - 1
    };
    lv_draw_image(&layer, &img_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

static void draw_graph(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 2);
    lv_point_t points[10];

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH_FIXED_RANGE)
    int max = CONFIG_NICE_OLED_WIDGET_WPM_GRAPH_FIXED_RANGE_MAX;
    if (max == 0) {
        max = 100;
    }

    int value = 0;
    for (int i = 0; i < 10; i++) {
        value = state->wpm[i];
        if (value > max) {
            value = max;
        }
        points[i].x = 0 + i * 7.4;
        points[i].y = 97 - (value * 32 / max);
    }
#else
    int max = 0;
    int min = 256;

    for (int i = 0; i < 10; i++) {
        if (state->wpm[i] > max) {
            max = state->wpm[i];
        }
        if (state->wpm[i] < min) {
            min = state->wpm[i];
        }
    }

    int range = max - min;
    if (range == 0) {
        range = 1;
    }

    for (int i = 0; i < 10; i++) {
        points[i].x = 0 + i * 7.4;
        points[i].y = 97 - (state->wpm[i] - min) * 32 / range;
    }
#endif

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    for (int i = 0; i < 9; i++) {
        line_dsc.p1.x = points[i].x;
        line_dsc.p1.y = points[i].y;
        line_dsc.p2.x = points[i + 1].x;
        line_dsc.p2.y = points[i + 1].y;
        lv_draw_line(&layer, &line_dsc);
    }
    lv_canvas_finish_layer(canvas, &layer);
}
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID) ||
       // !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_NUMBER)
static void draw_label(lv_obj_t *canvas, const struct status_state *state) {

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_VERTICAL) ||              \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_ONE_LINE_VERTICAL)

#define DRAW_LABEL_TEXT_ALIGN LV_TEXT_ALIGN_CENTER
#define DRAW_LABEL_FONTS &lv_font_montserrat_14
#define DRAW_LABEL_WMP "w:"
#define DRAW_LABEL_WMP_Y 110
#define DRAW_LABEL_WMP_X 12

#else

#define DRAW_LABEL_TEXT_ALIGN LV_TEXT_ALIGN_RIGHT
#define DRAW_LABEL_FONTS &pixel_operator_mono_16
#define DRAW_LABEL_WMP "WPM"
#define DRAW_LABEL_WMP_Y 103
#define DRAW_LABEL_WMP_X 26

#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_SYMBOL_VERTICAL) ||
       // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED_ONE_LINE_VERTICAL)

    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, DRAW_LABEL_FONTS, LV_TEXT_ALIGN_LEFT);
    label_left_dsc.text = DRAW_LABEL_WMP;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area1 = {.x1 = 0, .y1 = DRAW_LABEL_WMP_Y, .x2 = 24, .y2 = DRAW_LABEL_WMP_Y + 15};
    lv_draw_label(&layer, &label_left_dsc, &area1);

    lv_draw_label_dsc_t label_dsc_wpm;
    init_label_dsc(&label_dsc_wpm, LVGL_FOREGROUND, DRAW_LABEL_FONTS, DRAW_LABEL_TEXT_ALIGN);

    char wpm_text[6] = {};
    snprintf(wpm_text, sizeof(wpm_text), "%d", state->wpm[9]);
    label_dsc_wpm.text = wpm_text;

    lv_area_t area2 = {.x1 = DRAW_LABEL_WMP_X, .y1 = DRAW_LABEL_WMP_Y, .x2 = DRAW_LABEL_WMP_X + 41, .y2 = DRAW_LABEL_WMP_Y + 15};
    lv_draw_label(&layer, &label_dsc_wpm, &area2);
    lv_canvas_finish_layer(canvas, &layer);
}
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_NUMBER)

#else // CONFIG_NICE_EPAPER_ON
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_SPEEDOMETER)
static void draw_gauge(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &gauge;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = CONFIG_NICE_OLED_WIDGET_WPM_GAUGE_CUSTOM_X,
        .y1 = CONFIG_NICE_OLED_WIDGET_WPM_GAUGE_CUSTOM_Y,
        .x2 = CONFIG_NICE_OLED_WIDGET_WPM_GAUGE_CUSTOM_X + gauge.header.w - 1,
        .y2 = CONFIG_NICE_OLED_WIDGET_WPM_GAUGE_CUSTOM_Y + gauge.header.h - 1
    };
    lv_draw_image(&layer, &img_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

static void draw_needle(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 1);

    int centerX = CONFIG_NICE_OLED_WIDGET_WPM_NEEDLE_CENTER_CUSTOM_X;
    int centerY = CONFIG_NICE_OLED_WIDGET_WPM_NEEDLE_CENTER_CUSTOM_Y;
    int offset = 5;   // 5 def, largo de la aguja
    int value = state->wpm[9];

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH_FIXED_RANGE)
    float max = CONFIG_NICE_OLED_WIDGET_WPM_GRAPH_FIXED_RANGE_MAX;
#else
    float max = 0;
    for (int i = 0; i < 10; i++) {
        if (state->wpm[i] > max) {
            max = state->wpm[i];
        }
    }
#endif
    if (max == 0)
        max = 100;
    if (value < 0)
        value = 0;
    if (value > max)
        value = max;

    float radius = 25.45585;
    float angleDeg = 225 + ((float)value / max) * 90;
    float angleRad = angleDeg * (3.14159 / 180.0f);

    /* int needleStartX = 33 + (int)(13 * cos(4.71239)); // 33 + (int)(13 * 0) =
    33
    * int needleStartY = 66 + (int)(13 * sin(4.71239)); // 66 + (int)(13 * -1) =
    53
    * int needleEndX = 33 + (int)(25.45585 * cos(4.71239)); // 33 +
    (int)(25.45585 * 0) = 33
    * int needleEndY = 66 + (int)(25.45585 * sin(4.71239)); 66 + (int)(25.45585 *
    -1) = 40 */
    int needleStartX = centerX + (int)(offset * cos(angleRad));
    int needleStartY = centerY + (int)(offset * sin(angleRad));
    int needleEndX = centerX + (int)(radius * cos(angleRad));
    int needleEndY = centerY + (int)(radius * sin(angleRad));

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    line_dsc.p1.x = needleStartX;
    line_dsc.p1.y = needleStartY;
    line_dsc.p2.x = needleEndX;
    line_dsc.p2.y = needleEndY;
    lv_draw_line(&layer, &line_dsc);
    lv_canvas_finish_layer(canvas, &layer);
}
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_SPEEDOMETER)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_LUNA) ||                                                \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT) ||                                           \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED) ||                              \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH)
#else
static void draw_grid(lv_obj_t *canvas) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &grid;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = -1,
        .y1 = 95,
        .x2 = -1 + grid.header.w - 1,
        .y2 = 95 + grid.header.h - 1
    };
    lv_draw_image(&layer, &img_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

static void draw_graph(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_line_dsc_t line_dsc;
    init_line_dsc(&line_dsc, LVGL_FOREGROUND, 2);
    lv_point_t points[10];

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH_FIXED_RANGE)
    int max = CONFIG_NICE_OLED_WIDGET_WPM_GRAPH_FIXED_RANGE_MAX;
    if (max == 0) {
        max = 100;
    }

    int value = 0;
    for (int i = 0; i < 10; i++) {
        value = state->wpm[i];
        if (value > max) {
            value = max;
        }

        // modificar aqui par la posicion de la grafica
        points[i].x = -36 + i * 7.4;
        points[i].y = 127 - (value * 32 / max);
        // points[i].y = 132 - (value * 32 / max);
    }
#else
    int max = 0;
    int min = 256;

    for (int i = 0; i < 10; i++) {
        if (state->wpm[i] > max) {
            max = state->wpm[i];
        }
        if (state->wpm[i] < min) {
            min = state->wpm[i];
        }
    }

    int range = max - min;
    if (range == 0) {
        range = 1;
    }

    for (int i = 0; i < 10; i++) {
        points[i].x = 0 + i * 7.4;
        points[i].y = 97 - (state->wpm[i] - min) * 32 / range;
    }
#endif

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    for (int i = 0; i < 9; i++) {
        line_dsc.p1.x = points[i].x;
        line_dsc.p1.y = points[i].y;
        line_dsc.p2.x = points[i + 1].x;
        line_dsc.p2.y = points[i + 1].y;
        lv_draw_line(&layer, &line_dsc);
    }
    lv_canvas_finish_layer(canvas, &layer);
}
#endif

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_NUMBER)
static void draw_label(lv_obj_t *canvas, const struct status_state *state) {

    lv_draw_label_dsc_t label_dsc_wpm;
    init_label_dsc(&label_dsc_wpm, LVGL_FOREGROUND, &pixel_operator_mono_12, LV_TEXT_ALIGN_LEFT);

    char wpm_text[10] = {};
    snprintf(wpm_text, sizeof(wpm_text), "%d", state->wpm[9]);
    label_dsc_wpm.text = wpm_text;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);

    int x_offset = 0;
    if (state->wpm[9] < 10) {
        x_offset = 5;
    } else if (state->wpm[9] < 100) {
        x_offset = 2;
    }

    lv_area_t area = {
        .x1 = CONFIG_NICE_OLED_WIDGET_WPM_LABEL_CUSTOM_X + x_offset,
        .y1 = CONFIG_NICE_OLED_WIDGET_WPM_LABEL_CUSTOM_Y,
        .x2 = CONFIG_NICE_OLED_WIDGET_WPM_LABEL_CUSTOM_X + x_offset + 49,
        .y2 = CONFIG_NICE_OLED_WIDGET_WPM_LABEL_CUSTOM_Y + 11
    };
    lv_draw_label(&layer, &label_dsc_wpm, &area);
    lv_canvas_finish_layer(canvas, &layer);
}
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_NUMBER)
#endif // CONFIG_NICE_EPAPER_ON

void draw_wpm_status(lv_obj_t *canvas, const struct status_state *state) {
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON)
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_LUNA) ||                                                \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT) ||                                           \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_SPEEDOMETER)
#else
    draw_gauge(canvas, state);
    draw_needle(canvas, state);
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID) || !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH)
#else  // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID) ||
       // !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH)
    draw_grid(canvas);
    draw_graph(canvas, state);
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_RAW_HID) ||
       // !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_NUMBER)
    draw_label(canvas, state);
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_NUMBER)

#else // IS_ENABLED(CONFIG_NICE_EPAPER_ON)

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_SPEEDOMETER)
    draw_gauge(canvas, state);
    draw_needle(canvas, state);
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_SPEEDOMETER)
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_LUNA) ||                                                \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_BONGO_CAT) ||                                           \
    IS_ENABLED(CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_FIXED) ||                              \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_GRAPH)
#else
    draw_grid(canvas);
    draw_graph(canvas, state);
#endif
#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_NUMBER)
    draw_label(canvas, state);
#endif // IS_ENABLED(CONFIG_NICE_OLED_WIDGET_WPM_NUMBER)
#endif // IS_ENABLED(CONFIG_NICE_EPAPER_ON)
}
