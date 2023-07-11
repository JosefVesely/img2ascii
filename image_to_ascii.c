#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"
#include "stb_image/stb_image_resize.h"

char* characters = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

void print_usage(void)
{
    printf("Usage: image_to_ascii.exe --input image.png [--output ascii.txt] [--width 50] [--chars \"@#?|:. \"]\n");
    printf("  --help: shows this message\n");
    printf("  --input {image.png}: input file path\n");
    printf("  --output {ascii.txt}: output file path, \"output.txt\" if none (optional)\n");
    printf("  --width {50}: width of output (optional)\n");
    printf("  --chars {\"@#?|:. \"}: characters to be used (optional)\n");
}


int main(int argc, char** argv)
{
    // Exit if no command line arguments are given

    if (argc == 1) {
        print_usage();
        return EXIT_FAILURE;
    }

    // Parse arguments from the command line

    char* input_filepath;
    char* output_filepath = "output.txt";
    int desired_width;
    bool resize_image = false;

    struct option long_options[] =
    {
        {"help", no_argument, NULL, 'h'},
        {"input", required_argument, NULL, 'i'},
        {"output", optional_argument, NULL, 'o'},
        {"width", optional_argument, NULL, 'w'},
        {"chars", optional_argument, NULL, 'c'},
        {0, 0, 0, 0}
    };

    int c, option_index;
    char *short_options = "hi:o::w::c::";

    while ((c = getopt_long(argc, argv, short_options, long_options, NULL)) != EOF)
    {
        switch (c)
        {
        case 'h':
            print_usage();
            return EXIT_FAILURE;

        case 'i':
            input_filepath = optarg;
            break;

        case 'o':
            output_filepath = optarg;
            break;

        case 'w':
            desired_width = atoi(optarg);
            resize_image = true;
            break;

        case 'c':
            characters = optarg;
            break;
        }
    }

    // Load the image in grayscale

    int width, height;
    unsigned char* image = NULL;

    image = stbi_load(input_filepath, &width, &height, NULL, STBI_grey);

    if (image == NULL) {
        printf("Could not load image\n");
        return EXIT_FAILURE;
    }

    // Check if the width is a valid value

    if (resize_image && !(desired_width > 0 && desired_width <= width)) {
        printf("Argument \"width\" must be an integer between 1 and %d (the original image width)\n", width);
        return EXIT_FAILURE;
    }
    
    // Resize the image

    int desired_height;

    if (resize_image) {
        desired_height = height / (width / (float)desired_width);
        stbir_resize_uint8(image, width, height, width, image, desired_width, desired_height, desired_width, STBI_grey);
    }
    else {
        desired_width = width;
        desired_height = height;
    }

    // Create an output file

    FILE *file_pointer = NULL;

    file_pointer = fopen(output_filepath, "w");

    if (file_pointer == NULL) {
       printf("Could not create an output file\n");
       return EXIT_FAILURE;
    }

    int characters_count = strlen(characters);

    fprintf(
        file_pointer, 
        "Input: %s\n"
        "Output: %s\n"
        "Resolution: %dx%d\n"
        "Characters (%d): \"%s\"\n\n",
        input_filepath, output_filepath, desired_width, desired_height, characters_count, characters
    );

    for (int i = 0; i < desired_height*desired_width; i++) {
        int intensity = image[i];

        int character_index = intensity / (255 / (float)(characters_count - 1));

        fprintf(file_pointer, "%c", characters[character_index]);

        if ((i+1) % desired_width == 0) {
            fprintf(file_pointer, "\n");
        }
    }

    fclose(file_pointer);
    stbi_image_free(image);

    return EXIT_SUCCESS;
}