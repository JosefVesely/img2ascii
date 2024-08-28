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

struct Filepath {
    char *input;
    char *output;
};

struct ImageArea {
    int width;
    int height;
};

/*
 *
 */
void process_arguments(int argc,
                    char **argv,
                    struct Filepath *filepath,
                    struct ImageArea *area,
                    bool *resize_image,
                    char **characters,
                    uint8_t *flags);

#endif // ARGS_H
