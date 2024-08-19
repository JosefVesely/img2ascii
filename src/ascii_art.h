#ifndef ASCII_ART_H
#define ASCII_ART_H

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../stb_image/stb_image.h"
#include "../stb_image/stb_image_write.h"
#include "../stb_image/stb_image_resize.h"

#include <errno.h>
#include <math.h>
#include "utils.h"


uint8_t *load_image(const char *input_filepath, int *desired_width, int *desired_height, bool *resize_image)
{
    // Load the image

    const int channels = STBI_rgb;
    int width, height;

    uint8_t *image = stbi_load(input_filepath, &width, &height, NULL, channels);

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

        stbir_resize_uint8(
            image, width, height, width * channels, 
            image, *desired_width, *desired_height, *desired_width * channels, channels
        );
    }
    else {
        *desired_width = width;
        *desired_height = height;
    }
    return image;
}

void get_rgb(uint8_t *r, uint8_t *g, uint8_t *b, uint8_t *image, int i)
{
    const int channels = 3;
    uint8_t *pixel_offset = image + i * channels;

    *r = pixel_offset[0];
    *g = pixel_offset[1];
    *b = pixel_offset[2];
}

uint8_t get_intensity(uint8_t *image, int i)
{
    const int channels = 3;
    uint8_t *pixel_offset = image + i * channels;
    uint8_t r = pixel_offset[0];
    uint8_t g = pixel_offset[1];
    uint8_t b = pixel_offset[2];

    return (uint8_t)round(0.299 * r + 0.587 * g + 0.114 * b);
}

char *get_output_grayscale(uint8_t *image, int desired_width, int desired_height, char *characters, uint8_t flags) 
{
    if (flags & REVERSE_FLAG) {
        reverse_string(characters);
    }

    int output_size = desired_height * desired_width + desired_height + 1;
    char *output = (char *)malloc(output_size * sizeof(char));

    int characters_count = strlen(characters);

    int ptr = 0; // Index of the output string

    for (int i = 0; i < desired_height * desired_width; i++) {
        int intensity = get_intensity(image, i);

        int char_index = intensity / (255 / (float)(characters_count - 1));

        output[ptr] = characters[char_index];

        if ((i + 1) % desired_width == 0) {
            output[++ptr] = '\n';
        }
        ptr++;
    }
    output[ptr] = '\0';

    return output;
}

char *get_output_rgb(uint8_t *image, int width, int height, char *characters, uint8_t flags) 
{
    if (flags & REVERSE_FLAG) {
        reverse_string(characters);
    }

    int length = (height * width + height) * 21 + 1; // Length calculation
    char *output = (char *)malloc(length * sizeof(char));

    int characters_count = strlen(characters);
    int ptr = 0; // Index of the output string

    uint8_t r_prev, g_prev, b_prev;

    for (int i = 0; i < height * width; i++) {
        int intensity = get_intensity(image, i);
        int char_index = intensity / (255 / (float)(characters_count - 1));

        uint8_t r, g, b;
        get_rgb(&r, &g, &b, image, i);

        if (!(r == r_prev && g == g_prev && b == b_prev)) {
            ptr += snprintf(output + ptr, length - ptr, "\e[38;2;%i;%i;%im", r, g, b); // Append the ANSI code
        }
        r_prev = r;
        g_prev = g;
        b_prev = b;

        output[ptr++] = characters[char_index];

        if ((i + 1) % width == 0) {
            output[ptr++] = '\n';
        }
    }
    snprintf(output + ptr, length - ptr, "\e[0m"); // Append the ANSI reset code
    
    return output;
}

void write_output(
    uint8_t *image,
    char *input_filepath, 
    char *output_filepath, 
    char *characters, 
    int width,
    int height,
    uint8_t flags
) {
    char *output = NULL;

    if (flags & GRAYSCALE_FLAG) {
        output = get_output_grayscale(image, width, height, characters, flags);
    } else {
        output = get_output_rgb(image, width, height, characters, flags);
    }

    if (flags & DEBUG_FLAG) {
        printf(
            "Input: %s \n"
            "Output: %s \n"
            "Resolution: %ix%i \n"
            "Characters (%i): \"%s\" \n",
            input_filepath, 
            output_filepath != NULL ? output_filepath : "stdout",
            width, height, 
            (int)strlen(characters), characters
        );
    }

    if (flags & PRINT_FLAG) {
        printf("%s", output);
    }

    if (output_filepath != NULL) {
        FILE *file_ptr = NULL;

        errno = 0;
        file_ptr = fopen(output_filepath, "w");

        if (file_ptr == NULL) {
            fprintf(stderr, "Could not create an output file: %s \n", strerror(errno));
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