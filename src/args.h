#ifndef ARGS_H
#define ARGS_H

#include <stdlib.h>
#include <string.h>
#include <getopt.h>

#include "utils.h"


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
                *characters = (char*)realloc(*characters, strlen(optarg) + 1);
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

#endif // ARGS_H