#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"
#include "stb_image/stb_image_resize.h"

#define FILENAME_BUFFER_SIZE 256
const char* SYMBOLS = "@#$?!~:-.^` ";


int get_pixel_intensity(unsigned char* image, int image_width, int channels, int x, int y)
{
    unsigned char* pixel_offset = image + (image_width * x + y) * channels;
    unsigned char r = pixel_offset[0];
    unsigned char g = pixel_offset[1];
    unsigned char b = pixel_offset[2];

    return r + g + b;
}


int main(int argc, char** argv)
{
    // Parse arguments from the command line

    char input_filepath[FILENAME_BUFFER_SIZE];
    char output_filepath[FILENAME_BUFFER_SIZE];
    int desired_width;
    bool resize_image = true;

    if (argc == 1) {  // Usage
        printf("Usage: image_to_ascii.exe input [output] [width]\n");
        printf("   input: file path of the input image\n");
        printf("   output: file path of the output file (optional)\n");
        printf("   width: desired width of the output file (optional)\n");
        return EXIT_FAILURE;
    }

    if (argc >= 2) {  // input
        strcpy(input_filepath, argv[1]);
        input_filepath[strcspn(input_filepath, "\n")] = 0;  // Remove the newline character
    }

    if (argc >= 3) {  // output
        strcpy(output_filepath, argv[2]);
        output_filepath[strcspn(output_filepath, "\n")] = 0;
    }
    else
        strcpy(output_filepath, "output.txt");

    if (argc >= 4)  // width
        desired_width = atoi(argv[3]);
    else
        resize_image = false;

    // Load the image

    int width, height, channels;
    unsigned char* original_image = NULL;

    original_image = stbi_load(input_filepath, &width, &height, &channels, 0);

    if (original_image == NULL) {
        printf("Could not load image.\n");
        return EXIT_FAILURE;
    }

    // Check if is the input OK

    if (resize_image && !(desired_width > 0 && desired_width <= width)) {
        printf("'width' should be an integer between 1 and %d (the original image width)\n", width);
        return EXIT_FAILURE;
    }
    
    // Resize the image

    int desired_height;

    if (!resize_image) {
        desired_width = width;
        desired_height = height;
    }
    else {
        desired_height = height / (width / desired_width);
    }
    
    unsigned char* resized_image = malloc(desired_width * desired_height * channels);

    if (resized_image == NULL) {
        printf("Error! Could not allocate memory for the resized image.\n");
        return EXIT_FAILURE;
    }
    
    stbir_resize_uint8(original_image, width, height, width*channels, resized_image, desired_width, desired_height, desired_width*channels, channels);
    stbi_image_free(original_image);

    // Create output file

    FILE *file_pointer = NULL;

    if (output_filepath == NULL)
        file_pointer = fopen("output.txt", "w");
    else 
        file_pointer = fopen(output_filepath, "w");

    if (file_pointer == NULL)
    {
       printf("Error! Could not create an output file.\n");
       return EXIT_FAILURE;
    }

    fprintf(file_pointer, "%s [%dx%d]\n", input_filepath, desired_width, desired_height);


    for (int i = 0; i < desired_height; i++) {
        for (int j = 0; j < desired_width; j++)
        {
            int intensity = get_pixel_intensity(resized_image, desired_width, channels, i, j);

            int character_index = intensity / (3*255 / (strlen(SYMBOLS)-1));

            fprintf(file_pointer, "%c", SYMBOLS[character_index]);
        }
        fprintf(file_pointer, "\n");
    }

    // Free memory

    fclose(file_pointer);
    stbi_image_free(resized_image);

    return EXIT_SUCCESS;
}