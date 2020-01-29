Stephen Donchez

Dr. Wang

ECE 8448-DL1

29 January 2019

# Homework 2 README

## General Notes
-	All assembly files (excluding the auto-generated tutorial file) were authored using Microsoft Visual Studio Code, but should be viewable in any text editor
-	All Intel FPGA Monitor Program assets were generated using Intel FPGA Monitor Program Version 18.1 (December 2018). Program was installed alongside Quartus Prime Lite Edition, Version 18.1 with all current updates as of 28 January 2020
-	Programs are intended for execution on the Intel/Terasic DE1-SoC Development Board

If any questions arise, please feel free to contact me via email or phone:

sdonchez@villanova.edu

(484) 714-8517

## Part 1

### Important Files (in \Homework2\Part1)
-   __address_map_arm.s__ - provides map of hardware address for DE1_SoC board (for LED, switch, and push button access).
-   __getting_started.axf__ - contains both object code and debug information, to enable debugger to know contents of various memory locations.
-   __getting_started.s__ - Tutorial ARM assembly file, purpose and methodology indicated within.
-   __Homework2Part1.amp__ - XML based project information file
-   __makefile__ - instructions for the make executable (compiler wrapper)

### Notes
-    Had to change Terminal Device from semi-hosted to none, per figure 6 in Tutorial A
-    Compiler crashed and burned at the thought of a directory path that included spaces
-    Program file is unchanged from tutorial (no name, etc. included)

## Part 2

### Answer to Question (also in header comments in source code)
This program iterates through a list of N numbers to determine the largest value.

### Important Files
-   __Homework2Part2.axf__ - contains both object code and debug information, to enable debugger to know contents of various memory locations.
-   __Homework2Part2.s__ - Assignment ARM assembly file, purpose and methodology indicated within.
-   __Homework2Part2.amp__ - XML based project information file
-   __makefile__ - instructions for the make executable (compiler wrapper)

### Notes
-    Had to change Terminal Device from semi-hosted to none, per figure 6 in Tutorial A
-    Compiler crashed and burned at the thought of a directory path that included spaces

## Part 3

### Answer to Question (also in header comments in source code)
This program iterates through a list of N numbers to determine the largest value.

### Important Files
-   __Homework2Part3.axf__ - contains both object code and debug information, to enable debugger to know contents of various memory locations.
-   __Homework2Part3.s__ - Assignment ARM assembly file, purpose and methodology indicated within.
-   __Homework2Part3.amp__ - XML based project information file
-   __makefile__ - instructions for the make executable (compiler wrapper)

### Notes
-    Had to change Terminal Device from semi-hosted to none, per figure 6 in Tutorial A
-    Compiler crashed and burned at the thought of a directory path that included spaces
-    Sample code provided on Page 5 of assignment doesn't include a colon (:) after the Sub1 label, but the program failed to execute without it. The submitted file contains a colon.