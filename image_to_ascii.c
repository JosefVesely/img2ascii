#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h"
#include "stb_image/stb_image_resize.h"


void reverse_string(char *string) 
{
    int length = strlen(string);

    for (int i = 0; i < length / 2; i++) {
        char temp = string[i];
        string[i] = string[length - i - 1];
        string[length - i - 1] = temp;
    }
}


void show_usage(void)
{
    printf(
        "\nUsage: img2ascii [options] -i <FILE> [-o <FILE>] \n\n"

        "A command-line tool for converting images to ASCII art \n\n"

        "Options: \n"
        "   -i, --input  <FILE>     Path of the input image file (required) \n"
        "   -o, --output <FILE>     Path of the output file \n"
        "   -w, --width  <NUMBER>   Width of the output \n"
        "   -c, --chars  <STRING>   Characters to be used for the ASCII image \n"
        "   -p, --print             Print the output to the console \n"
        "   -r, --reverse           Reverse the string of characters \n"
    );
}


char *get_basename(const char *full_path) {
    char *last_slash = strrchr(full_path, '/');

    #ifdef _WIN32 // Backslashes are also directory separators on Windows
        char *last_backslash = strrchr(full_path, '\\');
        last_slash = (last_backslash > last_slash) ? last_backslash : last_slash;
    #endif

    if (last_slash != NULL) {
        last_slash++; // Skip the directory separator
    } else {
        last_slash = (char*)full_path; // No slash found, use the full path
    }

    char *last_dot = strrchr(last_slash, '.');

    // Length of the filename without extension
    size_t length = (last_dot != NULL) ? (int)(last_dot - last_slash) : strlen(last_slash);

    char *filename = (char*)malloc(length + 5);  // +4 for ".txt" and +1 for '\0'

    if (filename == NULL) {
        fprintf(stderr, "Memory allocation failed for filename \n");
        exit(EXIT_FAILURE);
    }

    strncpy(filename, last_slash, length);
    filename[length] = '\0';
    strcat(filename, ".txt");

    return filename;
}


void process_arguments(
    int argc, 
    char **argv, 
    char **input_filepath, 
    char **output_filepath, 
    bool *reverse_flag, 
    bool *print_flag,
    char **characters,
    int *desired_width,
    bool *resize_image
) {
    // Exit if no command line arguments are given

    if (argc == 1) {
        show_usage();
        exit(EXIT_FAILURE);
    }

    struct option long_options[] =
    {
        { "help",    no_argument,       NULL, 'h' },
        { "input",   required_argument, NULL, 'i' },
        { "output",  required_argument, NULL, 'o' },
        { "width",   required_argument, NULL, 'w' },
        { "chars",   required_argument, NULL, 'c' },
        { "print",   no_argument,       NULL, 'p' },
        { "reverse", no_argument,       NULL, 'r' },
        { 0, 0, 0, 0 }
    };

    int option;
    const char *short_options = "hi:o:w:c:pr";

    while ((option = getopt_long(argc, argv, short_options, long_options, NULL)) != EOF)
    {
        switch (option)
        {
        case 'h':
            show_usage();
            exit(EXIT_FAILURE);

        case 'i':
            *input_filepath = optarg;
            break;

        case 'o':
            *output_filepath = optarg;
            break;

        case 'w':
            *desired_width = atoi(optarg);
            *resize_image = true;
            break;

        case 'c':
            if (strlen(optarg) != 0) {
                *characters = realloc(*characters, strlen(optarg) + 1);
                strcpy(*characters, optarg);
            }
            break;

        case 'p':
            *print_flag = true;
            break;

        case 'r':
            *reverse_flag = true;
            break;
        
        case '?':
            exit(EXIT_FAILURE);
        }     
    }

    if (*input_filepath == NULL) {
        show_usage();
        exit(EXIT_FAILURE);
    }

    if (*output_filepath == NULL) {
        *output_filepath = get_basename(*input_filepath);
    }
}


unsigned char *load_image(const char *input_filepath, int *desired_width, int *desired_height, bool *resize_image)
{
    // Load the image in grayscale

    int width, height;
    unsigned char *image = NULL;

    image = stbi_load(input_filepath, &width, &height, NULL, STBI_grey);

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

        // Resize image
        *desired_height = height / (width / (float)*desired_width);
        stbir_resize_uint8(image, width, height, width, image, *desired_width, *desired_height, *desired_width, STBI_grey);
    }
    else {
        *desired_width = width;
        *desired_height = height;
    }    

    return image;
}


void write_ascii(
    int argc, 
    char **argv, 
    unsigned char *image,
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

    // Free memory
    stbi_image_free(image);
    free(output_filepath);
    free(characters);

    printf("File saved as '%s' \n", output_filepath);
}


int main(int argc, char **argv)
{
    // Parse arguments from the command line

    char *input_filepath = NULL;
    char *output_filepath = NULL;
    bool reverse_flag = false;
    bool print_flag = false;
    char *characters = strdup("$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ");
    int desired_width = 0;
    int desired_height = 0;
    bool resize_image = false;

    process_arguments(
        argc, 
        argv, 
        &input_filepath, 
        &output_filepath, 
        &reverse_flag, 
        &print_flag,
        &characters,
        &desired_width,
        &resize_image
    );

    // Load the image

    unsigned char *image = load_image(input_filepath, &desired_width, &desired_height, &resize_image);

    // Create an output file

    write_ascii(
        argc, 
        argv, 
        image,
        input_filepath, 
        output_filepath, 
        print_flag,
        reverse_flag, 
        characters, 
        desired_width,
        desired_height
    );

    

    return EXIT_SUCCESS;
}
