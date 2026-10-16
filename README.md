# 6502-emu

Basic NMOS 6502 Emulator implemented in C.

# Features (WIP)

- Cycle level accurate emulation
- Test suite for each instruction
- ROM loading support

# Usage

In the root directory of the project, compile and run the emulator with,

```
make
./6502-emu
```

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
  - Contains details about the 6602 ISA
  - Explains addressing modes, register purposes, memory, etc.
- [65x02 Processor Tests](https://github.com/SingleStepTests/65x02)
  - Language agnostic JSON test cases for each 6502 opcode
  - Details exact data/address bus and R/W statuses per clock cycle
- [6502_cpu.txt](https://www.nesdev.org/6502_cpu.txt)
  - Cycle-by-cycle breakdown of each instruction/addressing mode
- [cJSON](https://github.com/DaveGamble/cJSON)
  - Lightweight library for parsing JSON in C
  - Used for parsing the 65x02 JSON tests
