#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"
#include "stb_image/stb_image_resize.h"

char* symbols = "@#$?!~:-.^` ";


int main(int argc, char** argv)
{
    // Parse arguments from the command line

    char* input_filepath;
    char* output_filepath;
    int desired_width;
    bool resize_image = true;

    if (argc == 1) {  // Usage
        printf("Usage: image_to_ascii.exe input [output] [width]\n");
        printf("   input: file path of the image to convert\n");
        printf("   output: file path of the output file (optional)\n");
        printf("   width: width of output (optional)\n");
        printf("   chars: characters to be used (optional)\n");
        return EXIT_FAILURE;
    }

    if (argc >= 2) {  // input
        input_filepath = argv[1];
    }

    if (argc >= 3) {  // output
        output_filepath = argv[2];
    }
    else {
        output_filepath = "output.txt";
    }

    if (argc >= 4) {  // width
        desired_width = atoi(argv[3]);
    }
    else {
        resize_image = false;
    }

    if (argc >= 5) {
        symbols = argv[4];
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

    fprintf(file_pointer, "Input: %s  Output: %s  Resolution: [%dx%d]\n", input_filepath, output_filepath, desired_width, desired_height);

    for (int i = 0; i < desired_height*desired_width; i++) {
        int intensity = image[i];

        int character_index = intensity / (255 / (strlen(symbols)-1));

        fprintf(file_pointer, "%c", symbols[character_index]);

        if ((i+1) % desired_width == 0) {
            fprintf(file_pointer, "\n");
        }
    }

    fclose(file_pointer);
    stbi_image_free(image);

    return EXIT_SUCCESS;
}