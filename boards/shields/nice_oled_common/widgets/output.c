#include "output.h"
// #include "../assets/custom_fonts.h"
#include <fonts.h>
#include <zephyr/kernel.h>

LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static void draw_usb_connected(lv_obj_t *canvas) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &usb;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_X,
        .y1 = CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_Y,
        .x2 = CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_X + usb.header.w - 1,
        .y2 = CONFIG_NICE_OLED_WIDGET_OUTPUT_USB_CUSTOM_Y + usb.header.h - 1
    };
    lv_draw_image(&layer, &img_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

static void draw_ble_unbonded(lv_obj_t *canvas) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &bt_unbonded;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_X,
        .y1 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_Y,
        .x2 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_X + bt_unbonded.header.w - 1,
        .y2 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_UNBONDED_CUSTOM_Y + bt_unbonded.header.h - 1
    };
    lv_draw_image(&layer, &img_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}
#endif

static void draw_ble_disconnected(lv_obj_t *canvas) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &bt_no_signal;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X,
        .y1 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y,
        .x2 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X + bt_no_signal.header.w - 1,
        .y2 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y + bt_no_signal.header.h - 1
    };
    lv_draw_image(&layer, &img_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

static void draw_ble_connected(lv_obj_t *canvas) {
    lv_draw_image_dsc_t img_dsc;
    lv_draw_image_dsc_init(&img_dsc);
    img_dsc.src = &bt;

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t area = {
        .x1 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X,
        .y1 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y,
        .x2 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_X + bt.header.w - 1,
        .y2 = CONFIG_NICE_OLED_WIDGET_OUTPUT_BT_CUSTOM_Y + bt.header.h - 1
    };
    lv_draw_image(&layer, &img_dsc, &area);
    lv_canvas_finish_layer(canvas, &layer);
}

void draw_output_status(lv_obj_t *canvas, const struct status_state *state) {
#if IS_ENABLED(CONFIG_NICE_EPAPER_ON) &&                                                           \
    !IS_ENABLED(CONFIG_NICE_OLED_WIDGET_CENTRAL_SHOW_BATTERY_PERIPHERAL_ALL)
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono_16, LV_TEXT_ALIGN_LEFT);
    label_dsc.text = "SIG";

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t text_area = {.x1 = 0, .y1 = 1, .x2 = 24, .y2 = 1 + 16 - 1};
    lv_draw_label(&layer, &label_dsc, &text_area);

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_OUTPUT_BACKGROUND)
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);
    lv_area_t rect_area = {.x1 = 43, .y1 = 0, .x2 = 43 + 24 - 1, .y2 = 15 - 1};
    lv_draw_rect(&layer, &rect_white_dsc, &rect_area);
#endif
    lv_canvas_finish_layer(canvas, &layer);

#else

#if IS_ENABLED(CONFIG_NICE_OLED_WIDGET_OUTPUT_BACKGROUND)
    lv_draw_rect_dsc_t rect_white_dsc;
    init_rect_dsc(&rect_white_dsc, LVGL_FOREGROUND);

    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    lv_area_t rect_area = {.x1 = -3, .y1 = 32, .x2 = -3 + 24 - 1, .y2 = 32 + 15 - 1};
    lv_draw_rect(&layer, &rect_white_dsc, &rect_area);
    lv_canvas_finish_layer(canvas, &layer);
#endif

#endif // CONFIG_NICE_EPAPER_ON

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
    case ZMK_TRANSPORT_USB:
        draw_usb_connected(canvas);
        break;

    case ZMK_TRANSPORT_BLE:
        if (state->active_profile_bonded) {
            if (state->active_profile_connected) {
                draw_ble_connected(canvas);
            } else {
                draw_ble_disconnected(canvas);
            }
        } else {
            draw_ble_unbonded(canvas);
        }
        break;
    }
#else
    if (state->connected) {
        draw_ble_connected(canvas);
    } else {
        draw_ble_disconnected(canvas);
    }
#endif
}
