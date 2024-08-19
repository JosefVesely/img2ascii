#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "args.h"
#include "utils.h"
#include "ascii_art.h"


int main(int argc, char **argv)
{
    // Parse arguments from the command line

    char *input_filepath = NULL;
    char *output_filepath = NULL;
    char *characters = strdup("$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ");
    int desired_width = 0;
    int desired_height = 0;
    uint8_t flags = 0;
    bool resize_image = false;

    process_arguments(
        argc,
        argv,
        &input_filepath,
        &output_filepath,
        &characters,
        &desired_width,
        &flags,
        &resize_image
    );

    // Load the image

    uint8_t *image = load_image(input_filepath, &desired_width, &desired_height, &resize_image);

    // Create an output file

    write_output(
        image,
        input_filepath, 
        output_filepath, 
        characters, 
        desired_width,
        desired_height,
        flags
    );

    return EXIT_SUCCESS;
}