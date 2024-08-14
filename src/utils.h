#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void reverse_string(char *str) 
{
    int len = strlen(str);

    for (int i = 0; i < len / 2; i++) {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
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
        "   -r, --reverse           Reverse the string of characters \n\n"
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
    int length = (last_dot != NULL) ? (int)(last_dot - last_slash) : (int)strlen(last_slash);

    char *filename = (char*)malloc(length + 5);  // +4 for ".txt" and +1 for '\0'

    if (filename == NULL) {
        fprintf(stderr, "Could not allocate memory for filename \n");
        exit(EXIT_FAILURE);
    }

    strncpy(filename, last_slash, length);
    filename[length] = '\0';
    strcat(filename, ".txt");

    return filename;
}

#endif // UTILS_H