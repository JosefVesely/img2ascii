#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "args.h"
#include "utils.h"

void process_arguments(int argc,
                    char **argv,
                    struct Filepath *filepath,
                    struct ImageArea *area,
                    bool *resize_image,
                    char **characters,
                    uint8_t *flags)
{
    // Exit if no command line arguments are given

    if (argc == 1) {
        printf("No input file\n");
        show_usage();
        exit(EXIT_FAILURE);
    }

    struct option long_options[] = {
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

    while ((option = getopt_long(argc, argv, short_options, long_options, NULL)) != EOF) {
        switch (option) {
        case 'h':
            show_usage();
            exit(EXIT_FAILURE);

        case 'i':
            filepath->input = optarg;
            break;

        case 'o':
            filepath->output = optarg;
            break;

        case 'w':
            area->width = atoi(optarg);
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

    if (filepath->input == NULL) {
        printf("No input file\n");
        show_usage();
        exit(EXIT_FAILURE);
    }

    if (filepath->output == NULL) {
        *flags |= PRINT_FLAG;
    }
}

