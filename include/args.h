#ifndef ARGS_H
#define ARGS_H

#include <getopt.h>
#include <stdint.h>
#include <stdbool.h>

enum {
  GRAYSCALE_FLAG = 1 << 0,
  REVERSE_FLAG   = 1 << 1,
  PRINT_FLAG     = 1 << 2,
  DEBUG_FLAG     = 1 << 3
};

/*
 *
 */
void process_arguments(int argc, 
                       char **argv, 
                       char **input_filepath, 
                       char **output_filepath, 
                       char **characters,
                       int *desired_width,
                       uint8_t *flags,
                       bool *resize_image);

#endif // ARGS_H
