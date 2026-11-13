#include "cpu_macros.h"
#include "cpu_state.h"
#include "safe_wrappers.h"

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

// Alias to avoid verbosity
typedef struct option Option;

/// @brief Helper for displaying the helper message to avoid redundancy
/// @param prog_name Name of the program. Extract if with `argv[0]`
/// @param steam The output buffer being written to (usually `stderr` or `stdout`)
void print_usage(char* prog_name, FILE* stream) {
    fprintf(stream, "Usage: %s [options] bin_file\n", prog_name);
}

/// @brief Handler for processing and setting up emulator with CLI args
/// @param argc The number of CLI args
/// @param argv List of character buffers with CLI args in order
//  @param cmd_args Structure holding processed arguments for emulator configuration
void handle_args(int argc, char** argv, CmdArgs* cmd_args) {
    // Parse command line arguments
    int opt;
    int opt_idx = 0;

    Option long_options[] = {
        {"offset", required_argument, 0, 'o'},
        {"cycles", required_argument, 0, 'c'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0},
    };

    // Set defaults for processed cmd args
    cmd_args->bin_file = NULL;
    cmd_args->bin_offset = 0x8000;
    cmd_args->num_cycles = 100;

    // For converting strings
    char* endptr;
    long cycles, offset;

    // Check all CLI args
    while ((opt = getopt_long(argc, argv, "o:c:h", long_options, &opt_idx)) != -1) {
        endptr = NULL;
        switch (opt) {
        case 'o':
            // Convert the string rep. of cycles to long
            offset = strtol(optarg, &endptr, 16);
            if (optarg && *optarg != '\0' && *endptr == '\0') {
                // Conversion successful
                cmd_args->bin_offset = offset;
                break;
            }
            fprintf(stderr, "Error: Invalid hex offset '%s'\n", optarg);
            exit(EXIT_FAILURE);
        case 'c':
            // Convert the string rep. of cycles to long
            cycles = strtol(optarg, &endptr, 10);
            if (optarg && *optarg != '\0' && *endptr == '\0') {
                // Conversion successful
                cmd_args->num_cycles = cycles;
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
        cmd_args->bin_file = argv[optind];
    } else {
        fprintf(stderr, "Error: Missing required binary file (-b / --bin)\n");
        print_usage(argv[0], stderr);
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char** argv) {
    // Get emulation parameters
    CmdArgs cmd_args;
    handle_args(argc, argv, &cmd_args);

    // Create CPU
    Cpu cpu = {0};

    // Load ROM into CPU
    if (safe_read_file_bin(cmd_args.bin_file, &cpu, cmd_args.bin_offset)) {
        exit(EXIT_FAILURE); // Crash on I/O error
    }

    // Set hardware vectors ONLY IF binary file doesn't overwrite them
    if (cpu.mem[CPU_MEM_RESET_VEC_ADDR_L] == 0x00 && cpu.mem[CPU_MEM_RESET_VEC_ADDR_H] == 0x00) {
        cpu.mem[CPU_MEM_RESET_VEC_ADDR_L] = cmd_args.bin_offset & 0xFF;
        cpu.mem[CPU_MEM_RESET_VEC_ADDR_H] = cmd_args.bin_offset >> 8;
    }

    // Boot CPU
    cpu_reset(&cpu);

    // Fetch-decode-execute for specified number of cycles
    for (long n = 0; n < cmd_args.num_cycles; n++) {
        cpu_tick(&cpu);

        printf("Cycle %ld:\n", n);
        cpu_disp_state(&cpu);
    }

    return 0;
}
