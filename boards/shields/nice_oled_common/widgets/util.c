#include "util.h"
#include <ctype.h>
#include <zephyr/kernel.h>

void to_uppercase(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = toupper(str[i]);
  }
}

void rotate_canvas(lv_obj_t *canvas, uint8_t cbuf[]) {
  static uint8_t cbuf_tmp[CANVAS_BUF_SIZE];  // Fixed for LVGL 9: use uint8_t, not lv_color_t
  memcpy(cbuf_tmp, cbuf, sizeof(cbuf_tmp));

  lv_image_dsc_t img;
  img.data = (void *)cbuf_tmp;
  img.header.magic = LV_IMAGE_HEADER_MAGIC;
  img.header.cf = CANVAS_COLOR_FORMAT;  // Use consistent color format
  img.header.flags = 0;
  img.header.w = CANVAS_SIZE;
  img.header.h = CANVAS_SIZE;

  lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);

  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);

  lv_draw_image_dsc_t img_dsc;
  lv_draw_image_dsc_init(&img_dsc);
  img_dsc.src = &img;
  img_dsc.rotation = 900; /* 90 degrees in tenths */
  img_dsc.pivot.x = CANVAS_SIZE / 2;
  img_dsc.pivot.y = CANVAS_SIZE / 2;

  lv_area_t area = {.x1 = -1, .y1 = 0, .x2 = CANVAS_SIZE - 2, .y2 = CANVAS_SIZE - 1};
  lv_draw_image(&layer, &img_dsc, &area);

  lv_canvas_finish_layer(canvas, &layer);
}

void draw_background(lv_obj_t *canvas) {
  lv_draw_rect_dsc_t rect_black_dsc;
  init_rect_dsc(&rect_black_dsc, LVGL_BACKGROUND);

  lv_layer_t layer;
  lv_canvas_init_layer(canvas, &layer);
  lv_area_t area = {.x1 = 0, .y1 = 0, .x2 = CANVAS_WIDTH - 1, .y2 = CANVAS_HEIGHT - 1};
  lv_draw_rect(&layer, &rect_black_dsc, &area);
  lv_canvas_finish_layer(canvas, &layer);
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color,
                    const lv_font_t *font, lv_text_align_t align) {
  lv_draw_label_dsc_init(label_dsc);
  label_dsc->color = color;
  label_dsc->font = font;
  label_dsc->align = align;
}

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
  lv_draw_rect_dsc_init(rect_dsc);
  rect_dsc->bg_color = bg_color;
}

void init_line_dsc(lv_draw_line_dsc_t *line_dsc, lv_color_t color,
                   uint8_t width) {
  lv_draw_line_dsc_init(line_dsc);
  line_dsc->color = color;
  line_dsc->width = width;
}
