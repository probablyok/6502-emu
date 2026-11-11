#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/// @brief Struct for encapsulating simulation parameters set from command line
typedef struct {
    char* bin_file;  // Filepath to file representing assembled 6502 code
    long bin_offset; // Offset into CPU memory for placing binary file contents (given in base 16)
    long num_cycles; // The number of cycles to run the simulation for after resetting (given in
                     // base 10)
} CmdArgs;

/// Alias to avoid verbosity
typedef struct option Option;

void print_usage(char* prog_name, FILE* stream) {
    fprintf(stream, "Usage: %s [options] bin_file\n", prog_name);
}

int main(int argc, char** argv) {
    // Parse command line arguments
    int opt;
    int opt_idx = 0;

    Option long_options[] = {
        {"offset", required_argument, 0, 'o'},
        {"cycles", required_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0},
    };

    CmdArgs cmd_args = {
        .bin_file = NULL,
        .bin_offset = 0,
        .num_cycles = 100,
    };

    // For converting strings
    char* endptr;
    long cycles, offset;

    while ((opt = getopt_long(argc, argv, "o:c:h", long_options, &opt_idx)) != -1) {
        endptr = NULL;
        switch (opt) {
        case 'o':
            offset = strtol(optarg, &endptr, 16);
            if (optarg && *optarg != '\0' && *endptr == '\0') {
                // Conversion successful
                cmd_args.bin_offset = offset;
                break;
            }
            fprintf(stderr, "Error: Invalid hex offset '%s'\n", optarg);
            exit(EXIT_FAILURE);
        case 'c':
            // Convert the string rep. of cycles to long
            cycles = strtol(optarg, &endptr, 10);
            if (optarg && *optarg != '\0' && *endptr == '\0') {
                // Conversion successful
                cmd_args.num_cycles = cycles;
                break;
            }
            fprintf(stderr, "Error: Invalid cycle count '%s'\n", optarg);
            exit(EXIT_FAILURE);
            break;
        case 'h':
            print_usage(argv[0], stdout);
            printf("Simple NMOS 6502 emulator written in C\n\n");
            printf("-o, --offset\n\tspecifies the offset into CPU memory (in base 16) where the "
                   "program contents are loaded\n");
            printf("-c, --cycles\n\tspecifies the number of cycles (in base 10) the emulator runs "
                   "for before termination\n");
            exit(EXIT_SUCCESS);
        default:
            print_usage(argv[0], stderr);
            exit(EXIT_FAILURE);
        }
    }

    // Get binary file
    if (optind < argc) {
        cmd_args.bin_file = argv[optind];
    } else {
        fprintf(stderr, "Error: Missing required binary file (-b / --bin)\n");
        print_usage(argv[0], stderr);
        exit(EXIT_FAILURE);
    }

    return 0;
}
