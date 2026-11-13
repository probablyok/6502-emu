# kikuyu

Kikuyu is a basic NMOS 6502 Emulator implemented in C.

# Disclaimer

Kikuyu is a WIP emulator mainly intended for learning about the functionality of an NMOS 6502 CPU. This project is not open to pull requests, but please feel free to fork and use within the license specifications. 

# Usage


## Emulator
In the root directory of the project, compile and run the emulator with,

```
make
./kikuyu path_to_bin
```

where `path_to_bin` in some 6502 assembled binary the emulator runs. You can choose the offset into memory where the program is placed with `-o` (`0x8000` by default) and the number of cycles to execute for with `-c` (100 by default). The binary file provided does does not need to be padded to 16KiB. If the `RESET` vector is not set by the provided file, its set to the memory offset. 

Further information can be derived by running,

```
./kikuyu --help
```

You can clean any object/executable files made during the compilation with,

```
make clean
```

## Opcode Test Suite

Compile the test suite and run all tests with,

```
make test
./run_tests
```

The `./run_tests` script runs JSON tests for all opcodes when no arguments are specified. Opcodes (in hexadecimal) can be specified as arguments to perform specific opcode tests only. For example,

```
./run_tests a2 a0
```

would test `LDX #` and `LDY #` in that order.

Test cases are pulled from the [65x02 Processor Tests](https://github.com/SingleStepTests/65x02) repository, which takes up some 1.2GiB. You can clean this out with,

```
make clean-tests
```

# References

- [6502 Instruction Set](https://www.masswerk.at/6502/6502_instruction_set.html)
  - Contains details about the 6502 ISA
  - Explains addressing modes, register purposes, memory, etc.
- [65x02 Processor Tests](https://github.com/SingleStepTests/65x02)
  - Language agnostic JSON test cases for each 6502 opcode
  - Details exact data/address bus and R/W statuses per clock cycle
- [6502_cpu.txt](https://www.nesdev.org/6502_cpu.txt)
  - Cycle-by-cycle breakdown of each instruction/addressing mode
- [cJSON](https://github.com/DaveGamble/cJSON)
  - Lightweight library for parsing JSON in C
  - Used for parsing the 65x02 JSON tests
