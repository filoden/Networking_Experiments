# Networking Experiments

This repository contains a small set of educational C programs focused on core networking and data-link concepts. The projects demonstrate how binary data is processed in practical communication scenarios, with an emphasis on clarity and visualization.

## Projects Included

### CRC Calculator
A console-based program that shows how a cyclic redundancy check (CRC) codeword is generated using binary long division and XOR operations. It takes a binary dataword and a binary divisor, computes the remainder, and builds the final codeword.

### Bit Stuffing Demo
A terminal-based animation that visually demonstrates bit stuffing. The program generates random bit patterns, highlights runs of five consecutive 1s, and shows how an extra 0 is inserted to prevent ambiguity in framed data.

## Repository Structure

- CRC_Calculator/
  - CRC_Calculator.c: interactive CRC calculator program
  - CRC_Calc_win64.exe: prebuilt Windows executable
- Bit_Stuffing/
  - bit_stuffing.c: animated bit stuffing demo
  - bit_stuffing_win64.exe: prebuilt Windows executable

## Build Instructions

These programs are written in C and can be compiled with GCC.

### CRC Calculator

From the CRC_Calculator directory, build with:

```bash
gcc -Wall -Wextra -o CRC_Calculator CRC_Calculator.c
```

Run it with:

```bash
./CRC_Calculator
```

You will be prompted to enter a binary dataword and a binary divisor.

### Bit Stuffing Demo

From the Bit_Stuffing directory, build with:

```bash
gcc -Wall -Wextra -o bit_stuffing bit_stuffing.c
```

Run it with:

```bash
./bit_stuffing
```

> Note: the bit stuffing demo uses ANSI cursor control and sleeps between updates, so it works best in a terminal that supports escape sequences such as WSL, Git Bash, or a Unix-like shell.

## What You Will Learn

- How CRC works at a basic algorithmic level
- How bit stuffing prevents frame delimiter confusion
- How binary operations such as shifting and XOR are used in networking protocols

## Notes

These are educational examples rather than production-ready networking libraries. They are intended for experimentation, learning, and demonstration purposes.
