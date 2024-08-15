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
        "\nUsage: \e[1mimg2ascii [options] -i <FILE> [-o <FILE>]\e[0m \n\n"

        "A command-line tool for converting images to ASCII art \n\n"

        "Options: \n"
        "   -i, --input  <FILE>     Path of the input image file (required) \n"
        "   -o, --output <FILE>     Path of the output file \n"
        "   -w, --width  <NUMBER>   Width of the output \n"
        "   -c, --chars  <STRING>   Characters to be used for the ASCII image \n"
        "   -p, --print             Print the output to the console \n"
        "   -r, --reverse           Reverse the string of characters \n"
        "   -d, --debug             Print some useful information \n\n"
    );
}

#endif // UTILS_H