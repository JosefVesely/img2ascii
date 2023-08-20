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

char characters[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

void reverse_string(char* string) {
    int length = strlen(string);

    for (int i = 0; i < length / 2; i++) {
        char temp = string[i];
        string[i] = string[length - i - 1];
        string[length - i - 1] = temp;
    }
}

void print_usage(void)
{
    printf(
        "\nUsage: img2ascii --input=<FILE> [options] \n\n"

        "A command-line tool for converting images to ASCII art \n\n"

        "Options: \n"
        "   -i, --input=<FILE>      Path of the input image file (required) \n"
        "   -o, --output=<FILE>     Path of the output file (default is \"output.txt\") \n"
        "   -w, --width=<NUMBER>    Width of the output \n"
        "   -c, --chars=<STRING>    Characters to be used for the ASCII image \n"
        "   -p, --print             Print the output to the console \n"
        "   -r, --reverse           Reverse the string of characters \n\n"
    
        "GitHub Repository: https://github.com/JosefVesely/Image-to-ASCII \n\n"
    );
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
        { "help",    no_argument,           NULL, 'h' },
        { "input",   required_argument,     NULL, 'i' },
        { "output",  optional_argument,     NULL, 'o' },
        { "width",   optional_argument,     NULL, 'w' },
        { "chars",   optional_argument,     NULL, 'c' },
        { "print",   no_argument,  &print_flag,   'p' },
        { "reverse", no_argument,  &reverse_flag, 'r' },
        { 0, 0, 0, 0 }
    };

    int c;
    const char* short_options = "hi:o::w::c::pr";

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

    // Load the image in grayscale

    int width, height;
    unsigned char* image = NULL;

    image = stbi_load(input_filepath, &width, &height, NULL, STBI_grey);

    if (image == NULL) {
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
        stbir_resize_uint8(image, width, height, width, image, desired_width, desired_height, desired_width, STBI_grey);
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

    if (reverse_flag) {
        reverse_string(characters);
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
    stbi_image_free(image);

    return EXIT_SUCCESS;
}