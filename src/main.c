#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "ascii_art.h"

int main(int argc, char **argv)
{
    // Parse arguments from the command line
    char *characters = strdup("$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ");
    bool resize_image = false;
    uint8_t flags = 0;

    struct Filepath filepath = {
        .input  = NULL,
        .output = NULL,
    };

    struct ImageArea area = {
        .height = 0,
        .width  = 0,
    };

    process_arguments(argc,
                    argv,
                    &filepath,
                    &area,
                    &resize_image,
                    &characters,
                    &flags);

    // Load the image
    uint8_t *image = load_image(&filepath,
                                &area,
                                &resize_image);

    // Create an output file
    write_output(image,
                &filepath,
                &area,
                characters,
                flags);

    return EXIT_SUCCESS;
}
