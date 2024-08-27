#ifndef ASCII_ART_H
#define ASCII_ART_H

#include <stdint.h>
#include <stdbool.h>

/*
 *
 */
uint8_t get_intensity(uint8_t *image, int i);

/*
 *
 */
char *get_output_grayscale(uint8_t *image,
                           int desired_width,
                           int desired_height,
                           char *characters,
                           uint8_t flags);

/*
 *
 */
uint8_t *load_image(const char *input_filepath,
                    int *desired_width,
                    int *desired_height,
                    bool *resize_image);

/*
 *
 */
void write_output(uint8_t *image,
                  char *input_filepath, 
                  char *output_filepath, 
                  char *characters, 
                  int width,
                  int height,
                  uint8_t flags);

#endif // ASCII_ART_H
