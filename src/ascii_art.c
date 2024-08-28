#include <errno.h>
#include <math.h>

#define STB_IMAGE_RESIZE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image/stb_image_resize.h"
#include "stb_image/stb_image_write.h"
#include "stb_image/stb_image.h"

#include "ascii_art.h"
#include "error.h"
#include "color.h"
#include "utils.h"
#include "args.h"

uint8_t get_intensity(uint8_t *image, int i)
{
    const int channels = 3;
    uint8_t *pixel_offset = image + i * channels;
    uint8_t r = pixel_offset[0];
    uint8_t g = pixel_offset[1];
    uint8_t b = pixel_offset[2];

    return (uint8_t)round(0.299 * r + 0.587 * g + 0.114 * b);
}

char *get_output_grayscale(uint8_t *image,
                        struct ImageArea *area,
                        char *characters,
                        uint8_t flags)
{
    if (flags & REVERSE_FLAG) {
        reverse_string(characters);
    }

    int output_size = area->height * area->width + area->height + 1;
    char *output = (char *)malloc(output_size * sizeof(char));

    int characters_count = strlen(characters);

    int ptr = 0; // Index of the output string

    for (int i = 0; i < area->height * area->width; i++) {
        int intensity = get_intensity(image, i);

        int char_index = intensity / (255 / (float)(characters_count - 1));

        output[ptr] = characters[char_index];

        if ((i + 1) % area->width == 0) {
            output[++ptr] = '\n';
        }
        ptr++;
    }
    output[ptr] = '\0';

    return output;
}

uint8_t *load_image(struct Filepath *filepath,
                    struct ImageArea *area,
                    bool *resize_image)
{
    // Load the image
    const int channels = STBI_rgb;
    int width, height;

    uint8_t *image = stbi_load(filepath->input,
                            &width,
                            &height,
                            NULL,
                            channels);

    if (image == NULL) {
        error_info("Could not load image \n", 0, EXIT_FAILURE);
    }

    // Resize the image
    if (*resize_image) {
        // Check if the chosen width is valid
        if (area->width <= 0) {
            error_info("Argument 'width' must be greater than 0 \n", 0, EXIT_FAILURE);
        } 
        else if (area->width > width) {
            error_info("Argument 'width' can not be greater than the original image width (%ipx) \n",
                    0, EXIT_FAILURE);
        }

        area->height = height / (width / (float)area->width);

        stbir_resize_uint8(
            image, width, height, width * channels, 
            image, area->width, area->height, area->width * channels,
            channels
        );
    }
    else {
        area->width = width;
        area->height = height;
    }

    return image;
}

void write_output(uint8_t *image,
                struct Filepath *filepath,
                struct ImageArea *area,
                char *characters, 
                uint8_t flags)
{
    char *output = NULL;

    if (flags & GRAYSCALE_FLAG) {
        output = get_output_grayscale(image,
                                    area,
                                    characters,
                                    flags);
    }
    else {
        output = get_output_rgb(image, area->width, area->height, characters, flags);
    }

    if (flags & DEBUG_FLAG) {
        printf(
            "Input: %s \n"
            "Output: %s \n"
            "Resolution: %ix%i \n"
            "Characters (%i): \"%s\" \n",
            filepath->input, 
            filepath->output != NULL ? filepath->output : "stdout",
            area->width, area->height, 
            (int)strlen(characters), characters
        );
    }

    if (flags & PRINT_FLAG) {
        printf("%s", output);
    }

    if (filepath->output != NULL) {
        FILE *file_ptr = NULL;

        errno = 0;
        file_ptr = fopen(filepath->output, "w");

        if (file_ptr == NULL) {
            error_info("Could not create an output file", errno, EXIT_FAILURE);
        }
        fprintf(file_ptr, "%s", output);
    }

    // Free memory
    stbi_image_free(image);
    free(output);
    free(filepath->output);
    free(characters);
}
