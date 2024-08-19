#ifndef ARGS_H
#define ARGS_H

#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "utils.h"

enum {
    GRAYSCALE_FLAG = 1 << 0,
    REVERSE_FLAG   = 1 << 1,
    PRINT_FLAG     = 1 << 2,
    DEBUG_FLAG     = 1 << 3
};

void process_arguments(
    int argc, 
    char **argv, 
    char **input_filepath, 
    char **output_filepath, 
    char **characters,
    int *desired_width,
    uint8_t *flags,
    bool *resize_image
) {
    // Exit if no command line arguments are given

    if (argc == 1) {
        printf("No input file\n");
        show_usage();
        exit(EXIT_FAILURE);
    }

    struct option long_options[] =
    {
        { "help",      no_argument,       NULL, 'h' },
        { "input",     required_argument, NULL, 'i' },
        { "output",    required_argument, NULL, 'o' },
        { "width",     required_argument, NULL, 'w' },
        { "chars",     required_argument, NULL, 'c' },
        { "grayscale", no_argument,       NULL, 'g' },
        { "print",     no_argument,       NULL, 'p' },
        { "reverse",   no_argument,       NULL, 'r' },
        { "debug",     no_argument,       NULL, 'd' },
        { 0, 0, 0, 0 }
    };

    int option;
    const char *short_options = "hi:o:w:c:gprd";

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
                *characters = (char *)realloc(*characters, strlen(optarg) + 1);
                strcpy(*characters, optarg);
            }
            break;

        case 'g':
            *flags |= GRAYSCALE_FLAG;
            break;

        case 'p':
            *flags |= PRINT_FLAG;
            break;

        case 'r':
            *flags |= REVERSE_FLAG;
            break;

        case 'd':
            *flags |= DEBUG_FLAG;
            break;
        
        case '?':
            printf("\nHint: Use the \e[1m--help\e[0m option to get help about the usage \n\n");
            exit(EXIT_FAILURE);
        }
    }

    if (*input_filepath == NULL) {
        printf("No input file\n");
        show_usage();
        exit(EXIT_FAILURE);
    }

    if (*output_filepath == NULL) {
        *flags |= PRINT_FLAG;
    }
}

#endif // ARGS_H