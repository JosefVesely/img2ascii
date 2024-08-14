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

void write_output(
    int argc, 
    char **argv, 
    uint8_t *image,
    char *input_filepath, 
    char *output_filepath, 
    bool print_flag,
    bool reverse_flag, 
    char *characters, 
    int desired_width,
    int desired_height
) {
    FILE *file_pointer = NULL;

    file_pointer = fopen(output_filepath, "w");

    if (file_pointer == NULL) {
        fprintf(stderr, "Could not create an output file \n");
        exit(EXIT_FAILURE);
    }

    if (reverse_flag) {
        reverse_string(characters);
    }

    // Write some useful information to the file

    fprintf(file_pointer, "img2ascii ");

    for (int i = 1; i < argc; i++) {
        fprintf(file_pointer, "%s ", argv[i]);
    }

    int characters_count = strlen(characters);

    fprintf(
        file_pointer,
        "\n\n"
        "Input: %s \n"
        "Output: %s \n"
        "Resolution: %ix%i \n"
        "Characters (%i): \"%s\" \n\n",
        input_filepath, 
        output_filepath, 
        desired_width, desired_height, 
        characters_count, characters
    );

    // Write the ASCII art to the file

    for (int i = 0; i < desired_height * desired_width; i++) {
        int intensity = image[i];

        int character_index = intensity / (255 / (float)(characters_count - 1));

        if (print_flag) {
            putchar(characters[character_index]);
        }
        fputc(characters[character_index], file_pointer);

        if ((i + 1) % desired_width == 0) {
            if (print_flag) {
                putchar('\n');
            }
            fputc('\n', file_pointer);
        }
    }

    fclose(file_pointer);

    printf("File saved as '%s' \n", output_filepath);

    // Free memory
    stbi_image_free(image);
    free(output_filepath);
    free(characters);
}

#endif // ASCII_ART_H