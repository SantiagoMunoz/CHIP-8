# CHIP-8

A WIP CHIP-8 emulator and compiler made in C.

NOTE: Im working on this in my spare time, be patient.

Take a look at the technical reference [here](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

## Usage

Simply run the program specifying the program you want to load as an argument

    $ ./chip8 program.chip8


## Tests

To run all tests:

	$ ctest

To run individual tests:

	$ test/./c8_intructions_tests

## Compiler

To use the compiler, pass the source file and the desired output file as arguments
    
    $ ./chip8c input.src output.chip8

### How to insert data into your programs

The compiler output is loaded by the chip8 interpreter starting at location 0x200 (initial location of the program counter).
To insert a data segment, use the keyword '!DATA' plus a valid address after you are done with the code segment:

    !DATA <ADDRESS>
    <16 bit word>
    <16 bit word>
    <16 bit word>
    ...

The 16 bit words you input after '!DATA <ADDRESS>' are copied as a block into memory.
You may specify more than one '!DATA' segment to keep you memory organized, but you shall not try to overwrite already set memory positions.

Ex:

    !DATA 270
    A4BB
    B0CC
    11DD

    !DATA 3F0
    1911
    B022
    D733
    FF44
     
Note: Remember that all numbers are hex and that each instruction is 16 bit long

## Compiler tests

The 'test' folder contains a sample program all_instructions.src and a reference.txt file that contains the expected compilation result.
Compile the program with:
    
    $ ./chip8c test/all_instructions.src test/all_instructions.chip8

Then run the check program:

    $ ./compiler_test

