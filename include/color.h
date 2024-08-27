#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>

struct ColorModel {
  uint8_t *r;
  uint8_t *g;
  uint8_t *b;
};

/*
 *
 */
char *get_output_rgb(uint8_t *image,
                     int width,
                     int height,
                     char *characters,
                     uint8_t flags);

/*
 *
 */
void get_rgb(struct ColorModel *color,
             uint8_t *image,
             int i);

#endif // COLOR_H
