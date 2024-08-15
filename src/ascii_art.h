#ifndef ASCII_ART_H
#define ASCII_ART_H

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../stb_image/stb_image.h"
#include "../stb_image/stb_image_write.h"
#include "../stb_image/stb_image_resize.h"

#include "utils.h"


uint8_t *load_image(const char *input_filepath, int *desired_width, int *desired_height, bool *resize_image)
{
    // Load the image in grayscale

    int width, height;
    uint8_t *image = stbi_load(input_filepath, &width, &height, NULL, STBI_grey);

    if (image == NULL) {
        fprintf(stderr, "Could not load image \n");
        exit(EXIT_FAILURE);
    }

    // Resize the image

    if (*resize_image) {
        // Check if the chosen width is valid
        if (*desired_width <= 0) {
            fprintf(stderr, "Argument 'width' must be greater than 0 \n");
            exit(EXIT_FAILURE);
        } 
        else if (*desired_width > width) {
            fprintf(stderr, "Argument 'width' can not be greater than the original image width (%ipx) \n", width);
            exit(EXIT_FAILURE);
        }

        *desired_height = height / (width / (float)*desired_width);
        stbir_resize_uint8(image, width, height, width, image, *desired_width, *desired_height, *desired_width, STBI_grey);
    }
    else {
        *desired_width = width;
        *desired_height = height;
    }
    return image;
}

char *get_ascii_image(const uint8_t *image, int desired_width, int desired_height, char *characters, bool reverse_flag) 
{
    if (reverse_flag) {
        reverse_string(characters);
    }

    // Print the ASCII art to the file

    int output_size = desired_height * desired_width + desired_height + 1;
    char *output = (char *)malloc(output_size * sizeof(char));

    int characters_count = strlen(characters);

    int ptr = 0; // Index of the output string

    for (int i = 0; i < desired_height * desired_width; i++) {
        int intensity = image[i];

        int character_index = intensity / (255 / (float)(characters_count - 1));

        output[ptr] = characters[character_index];

        if ((i + 1) % desired_width == 0) {
            output[++ptr] = '\n';
        }
        ptr++;
    }
    output[ptr] = '\0';

    return output;
}

void write_output(
    uint8_t *image,
    char *input_filepath, 
    char *output_filepath, 
    char *characters, 
    int desired_width,
    int desired_height,
    bool print_flag,
    bool reverse_flag,
    bool debug_flag
) {
    char *output = get_ascii_image(image, desired_width, desired_height, characters, reverse_flag);

    if (debug_flag) {
        printf(
            "Input: %s \n"
            "Output: %s \n"
            "Resolution: %ix%i \n"
            "Characters (%i): \"%s\" \n",
            input_filepath, 
            output_filepath != NULL ? output_filepath : "stdout",
            desired_width, desired_height, 
            strlen(characters), characters
        );
    }

    if (print_flag) {
        printf("%s", output);
    }

    if (output_filepath != NULL) {
        FILE *file_ptr = NULL;

        file_ptr = fopen(output_filepath, "w");

        if (file_ptr == NULL) {
            fprintf(stderr, "Could not create an output file \n");
            exit(EXIT_FAILURE);
        }
        fprintf(file_ptr, "%s", output);
    }

    // Free memory
    stbi_image_free(image);
    free(output);
    free(output_filepath);
    free(characters);
}

#endif // ASCII_ART_H