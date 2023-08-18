#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"
#include "stb_image/stb_image_resize.h"

#define NORMAL  "\x1B[0m"
#define WHITE   "\x1B[37m"
#define RED     "\x1B[31m"
#define GREEN   "\x1B[32m"
#define BLUE    "\x1B[34m" 
#define YELLOW  "\x1B[33m"
#define MAGENTA "\x1B[35m"
#define CYAN    "\x1B[36m"

char characters[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

void print_usage(void)
{
    printf(
        "Usage: img2ascii --input=image.png [--output=ascii.txt] [--width=50] [--chars=\"@#?|:. \"] \n"
        "  --help: shows this message \n"
        "  --input={image.png}: input file path \n"
        "  --output={ascii.txt}: output file path, \"output.txt\" if none (optional) \n"
        "  --width={50}: width of output (optional) \n"
        "  --chars={\"@#?|:. \"}: characters to be used (optional) \n"
        "  --print: print the output to the console \n"
        "  --reverse: reverse the string of characters \n"
    );
}

typedef struct 
{
    int r, g, b;
} Color;

void print_closest_matching_color(Color color)
{
    int colors[][3] = {
        { 255, 255, 255 },  // WHITE
        { 0, 0, 0 },        // BLACK
        { 255, 0, 0 },      // RED
        { 0, 255, 0 },      // GREEN
        { 0, 0, 255 },      // BLUE
        { 255, 255, 0},     // YELLOW
        { 255, 0, 255 },    // MAGENTA
        { 0, 255, 255 }     // CYAN
    };

    float difference = 0.f;
    float smallest_difference = 1000.f;
    int smallest_index = 0;
    
    for (int i = 0; i < 8; i++) {
        int r = colors[i][0];
        int g = colors[i][1];
        int b = colors[i][2];

        float difference = sqrt(pow(color.r - r, 2) + pow(color.g - g, 2) + pow(color.b - b, 2)); // Euclidean distance

        if (difference < smallest_difference) {
            smallest_difference = difference;
            smallest_index = i;
        }
    }

    //printf("RGB: %d %d %d \t Match #: %d \n", color.r, color.g, color.b, smallest_index);

    switch (smallest_index)
    {
    case 0: case 1: printf(WHITE); break;
    case 2: printf(RED);     break;  
    case 3: printf(GREEN);   break;  
    case 4: printf(BLUE);    break;  
    case 5: printf(YELLOW);  break;  
    case 6: printf(MAGENTA); break;  
    case 7: printf(CYAN);    break;  
    }
}



Color get_pixel_rgb(unsigned char* image, int image_width, int i)
{
    unsigned char* pixel_offset = image + i * 3;
    unsigned char r = pixel_offset[0];
    unsigned char g = pixel_offset[1];
    unsigned char b = pixel_offset[2];

    Color color = { r, g, b };

    return color;
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
    int reverse_flag = 0;
    int print_flag = 0;
    int resize_image = false;
    int desired_width;

    struct option long_options[] =
    {
        { "help",   no_argument,         NULL, 'h' },
        { "input",  required_argument,   NULL, 'i' },
        { "output", optional_argument,   NULL, 'o' },
        { "width",  optional_argument,   NULL, 'w' },
        { "chars",  optional_argument,   NULL, 'c' },
        { "reverse", no_argument, &reverse_flag, 'r' },
        { "print",  no_argument,  &print_flag, 'p' },
        { 0, 0, 0, 0 }
    };

    int c;
    const char* short_options = "hi:o::w::c::i";

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
            if (strlen(optarg) != 0) {
                strcpy(characters, optarg);
            }
            break;
        }
    }

    // Load the image in RGB and grayscale

    int width, height;
    unsigned char* image_rgb = NULL;
    unsigned char* image_gray = NULL;

    image_rgb = stbi_load(input_filepath, &width, &height, NULL, STBI_rgb);
    image_gray = stbi_load(input_filepath, &width, &height, NULL, STBI_grey);

    if (image_rgb == NULL || image_gray == NULL) {
        fprintf(stderr, "Could not load image \n");
        return EXIT_FAILURE;
    }

    // Check if the width is a valid value

    if (resize_image && !(desired_width > 0 && desired_width <= width)) {
        fprintf(stderr, "Argument \"width\" must be an integer between 1 and %d (the original image width) \n", width);
        return EXIT_FAILURE;
    }
    else if (desired_width == width) {
        resize_image = false;
    }
    
    // Resize the image

    int desired_height;

    if (resize_image) {
        desired_height = height / (width / (float)desired_width);
        stbir_resize_uint8(image_rgb, width, height, 0, image_rgb, desired_width, desired_height, 0, STBI_rgb);
        stbir_resize_uint8(image_gray, width, height, 0, image_gray, desired_width, desired_height, 0, STBI_grey);

        // DEBUG
        stbi_write_png("DEBUG.png", desired_width, desired_height, 3, image_rgb, desired_width * 3);

    }
    else {
        desired_width = width;
        desired_height = height;
    }

    // Create an output file

    FILE* file_pointer = NULL;

    file_pointer = fopen(output_filepath, "w");

    if (file_pointer == NULL) {
       fprintf(stderr, "Could not create an output file \n");
       return EXIT_FAILURE;
    }

    if (!reverse_flag) {
        strrev(characters);
    }

    int characters_count = strlen(characters);

    fprintf(
        file_pointer, 
        "Input: %s \n"
        "Output: %s \n"
        "Resolution: %dx%d \n"
        "Characters (%d): \"%s\"\n \n",
        input_filepath, output_filepath, desired_width, desired_height, characters_count, characters
    );

    for (int i = 0; i < desired_height * desired_width; i++) {
        int intensity = image_gray[i];

        int character_index = intensity / (255 / (float)(characters_count - 1));

        if (print_flag) {
            Color color = get_pixel_rgb(image_rgb, desired_width, i);
            print_closest_matching_color(color);
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
    printf(NORMAL);

    fclose(file_pointer);
    
    stbi_image_free(image_rgb);
    stbi_image_free(image_gray);

    return EXIT_SUCCESS;
}