#ifndef ASCII_ART_H
#define ASCII_ART_H

#include <stdint.h>
#include <stdbool.h>

#include "args.h"

/*
 *
 */
uint8_t get_intensity(uint8_t *image, int i);

/*
 *
 */
char *get_output_grayscale(uint8_t *image,
                        struct ImageArea *area,
                        char *characters,
                        uint8_t flags);

/*
 *
 */
uint8_t *load_image(struct Filepath *filepath,
                    struct ImageArea *area,
                    bool *resize_image);

/*
 *
 */
void write_output(uint8_t *image,
                struct Filepath *filepath,
                struct ImageArea *area,
                char *characters, 
                uint8_t flags);

#endif // ASCII_ART_H
