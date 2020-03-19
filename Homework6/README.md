Stephen Donchez

Dr. Wang

ECE 8448-DL1

18 March 2020

# Homework 6 README

## Assignment Description
See HW6.pdf for Assignment Description

## General Notes
-   __This program makes use of the audio device drivers provided by the university program. For successful execution, they must be configured as outlined in "Using Linux on DE-Series Boards", page 24!__
    -   __prior to execution, execute the command "/home/root/Linux_Libraries/drivers/load_drivers"__
-   All source files (excluding the auto-generated tutorial file) were authored using Microsoft Visual Studio 2019, but should be viewable in any text editor
-	Execution environment is intel linux image for DE1-SoC
-	Programs are intended for execution on the Intel/Terasic DE1-SoC Development Board

If any questions arise, please feel free to contact me via email or phone:

sdonchez@villanova.edu

(484) 714-8517

## Part 1

### Important Files (in \Homework6\Part1)
-   __main.c__ - main routine and tone generation subroutinesinputs
-   __include\tone.h__ - defines tones as outlined in project assignment
-   __bin\ARM\Release\Part1.out__ precompiled executable (same is found in bin\ARM\debug)

### Notes
-    to compile, execute command __"gcc main.c -o Part1.out -lintelfpgaup -lm -std=c99"__

## Part 2

### Important Files (in \Homework6\Part2)
-   __main.c__ - main routine and tone generation subroutinesinputs
-   __include\tone.h__ - defines tones as outlined in project assignment
-   __bin\ARM\Release\Part2.out__ precompiled executable (same is found in bin\ARM\debug)

### Notes
-    to compile, execute command __"gcc main.c -o Part2.out -lintelfpgaup -lm -std=c99"__