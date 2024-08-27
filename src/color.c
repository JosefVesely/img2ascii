#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ascii_art.h"
#include "color.h"
#include "utils.h"
#include "args.h"

char *get_output_rgb(uint8_t *image,
                     int width,
                     int height,
                     char *characters,
                     uint8_t flags)
{
  if (flags & REVERSE_FLAG) {
    reverse_string(characters);
  }

  int length = (height * width + height) * 21 + 1; // Length calculation
  char *output = (char *)malloc(length * sizeof(char));

  int characters_count = strlen(characters);
  int ptr = 0; // Index of the output string

  uint8_t r_prev, g_prev, b_prev;

  for (int i = 0; i < height * width; i++) {
    int intensity = get_intensity(image, i);
    int char_index = intensity / (255 / (float)(characters_count - 1));

    struct ColorModel color = {
      .r = 0,
      .g = 0,
      .b = 0,
    };
    get_rgb(&color, image, i);

    if (!(*color.r == r_prev && *color.g == g_prev && *color.b == b_prev)) {
      ptr += snprintf(output + ptr, length - ptr, "\e[38;2;%i;%i;%im",
          *color.r, *color.g, *color.b); // Append the ANSI code
    }
    r_prev = *color.r;
    g_prev = *color.g;
    b_prev = *color.b;

    output[ptr++] = characters[char_index];

    if ((i + 1) % width == 0) {
      output[ptr++] = '\n';
    }
  }
  snprintf(output + ptr, length - ptr, "\e[0m"); // Append the ANSI reset code
  
  return output;
}

void get_rgb(struct ColorModel *color,
             uint8_t *image,
             int i)
{
  const int channels = 3;
  uint8_t *pixel_offset = image + i * channels;

  *color->r = pixel_offset[0];
  *color->g = pixel_offset[1];
  *color->b = pixel_offset[2];
}

