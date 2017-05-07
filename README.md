# nand2tetris
This repository consists of a set of projects based on the original nand2tetris course: [nand2tetris Website](http://www.nand2tetris.org/).
The aim of these projects is to implement a full scale 16 bit computer along with essential software (OS, assembler, compiler...)
from scratch. The basic building block is a NAND logic gate and it is used to further create more complex logic gates up to a read 16 bit processor and an assembler for the processor instruction set architecture. It continues with software-related projects such as an assembler, a virtual machine, a compiler and an OS. The final task of this course, is to create a game on the programming language you built yourself, running on the computer, which is your deed as well.

The included projects are:
## Hack Computer
Using a hardware description language, I have designed all parts of a full scale 16 bit computer called Hack.
It starts of with designing simple logic gates (AND, OR, NOR...), then moves on to more complex ones (Mux, Demux...).
Afterwards, I use the already created elements to build an adder, an ALU, a main memory and a processor. 
Finally, the Hack Computer chip is assembled and can be used as an abstraction for further projects.

## Hack Assembler
A program, which translates mnemonics to machine code. This project follows the Hack Language specification presented in 
chapter 4 of the nand2tetris book: [The Elements of Computing Systems, Chapter 4](http://nand2tetris.org/chapters/chapter%2004.pdf)

## VMTranslator
After building the assembler, a virtual machine is built in order to allow a two-tier compilation (Just like in Java and C#).
For this purpose, the next project translates intermediate VM code to the assembly language presented before.
The VM language specification can be found at: [The Elements of Computing Systems, Chapter 7](http://www1.idc.ac.il/tecs/book/chapter07.pdf)

## Jack Compiler
A compiler that translates a high-level programming language called Jack (Syntax similar to Java), to the intermediate VM language.
The language is described using a syntax grammar, specified in: [The Elements of Computing Systems, Chapter 10](http://www1.idc.ac.il/tecs/book/chapter10.pdf)

## Operating system
An OS, designed to provide utility functions, used by the compiler such as allocating dynamic memory and bootloading the initial code.
It also provides a standard library, similar to other popular libraries. 
It consists of utilities for Math functions, handling I/O with the screen and keyboard, accesing raw memory and more.

The tools required to work with these projects can be found at: [nand2tetris Software Suite](http://www.nand2tetris.org/software.php)
