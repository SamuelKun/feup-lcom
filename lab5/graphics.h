#include <lcom/lcf.h>
#include <lcom/lab5.h>

#include <stdint.h>

int (vbe_mode_info)(uint16_t mode, vbe_mode_info_t *vbe);

int (vg_init_function)(uint16_t mode);

void (draw_pixel)(uint16_t x, uint16_t y, const void *color);

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (draw_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step);

int (draw_xpm)(uint8_t *sprite, uint16_t x, uint16_t y, uint8_t width, uint8_t height);

int (clear_screen)();

int read_vbe(vg_vbe_contr_info_t *info_p);
