# Hack Assembler

An assembler for the Hack assembly language from the Nand2Tetris Project 6.

## Description

This project translates Hack assembly programs (`.asm`) into Hack machine code (`.hack`).

The assembler performs two passes:

1. First pass resolves labels and builds the symbol table.
2. Second pass translates A-instructions and C-instructions into binary machine code.

## Features

- A-instruction translation
- C-instruction translation
- Predefined symbols
- Label resolution
- Variable allocation
- Comment and whitespace removal

## Built With

- C++
- C++ Standard Library

## Nand2Tetris

This project was built as part of [Nand2Tetris](https://www.nand2tetris.org/), Project 6.

## Status

Completed and passes the official Nand2Tetris assembler tests.