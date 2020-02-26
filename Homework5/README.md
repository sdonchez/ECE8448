Stephen Donchez

Dr. Wang

ECE 8448-DL1

26 February 2020

# Homework 5 README

## Assignment Description
See HW5.pdf for Assignment Description

## General Notes
-	All source files (excluding the auto-generated tutorial file) were authored using Microsoft Visual Studio 2019, but should be viewable in any text editor
-	All Intel FPGA Monitor Program assets were generated using Intel FPGA Monitor Program Version 18.1 (December 2018). Program was installed alongside Quartus Prime Lite Edition, Version 18.1 with all current updates as of 5 February 2020
-	Programs are intended for execution on the Intel/Terasic DE1-SoC Development Board
-   __I had a weird issue in setting up my monitor program projects - if I created a blank project and inserted my code, make would crash and complain that it didn't have a target to build the header files - I could see this in the makefile, but the .amp (xml) file looked right. The only way I could get my programs to build was to setup the interrupt sample program, delete the original code, and replace it with my code.__

If any questions arise, please feel free to contact me via email or phone:

sdonchez@villanova.edu

(484) 714-8517

## Part 1

### Important Files (in \Homework5\Part1)
-   __main.c__ - main routine and configuration subroutines
-   __exceptions.c__ - subroutines for configuring and handling interrupts
-   __pushbutton_ISR.c__ - provides the interrupt service routine for the KEY inputs
-   __timer0_ISR.c__ - provides the interrupt service routine for the HPS_TIMER0
-   __address_map_arm.h__ - provides address values that exist in the system.
-   __defines.h__ - provides definitions for common values that exist in the system
-   __interruptID.h__ - provides interrupt IDs
-   __Homework5Part1.amp__ - XML based project information file
-   __makefile__ - instructions for the make executable (compiler wrapper)

### Notes
-    Much of the code is unmodified, this is indicated in file and function header blocks.
-    Unmodified code is sourced from the Monitor Program's interrupts sample program.

## Part 2

### Important Files (in \Homework5\Part2)
-   __main.c__ - main routine and configuration subroutines
-   __exceptions.c__ - subroutines for configuring and handling interrupts
-   __pushbutton_ISR.c__ - provides the interrupt service routine for the KEY inputs
-   __timer0_ISR.c__ - provides the interrupt service routine for the HPS_TIMER0
-   __address_map_arm.h__ - provides address values that exist in the system.
-   __defines.h__ - provides definitions for common values that exist in the system
-   __interruptID.h__ - provides interrupt IDs
-   __Homework5Part1.amp__ - XML based project information file
-   __makefile__ - instructions for the make executable (compiler wrapper)

### Notes
-    Much of the code is unmodified, this is indicated in file and function header blocks.
-    Unmodified code is sourced from the Monitor Program's interrupts sample program.
-    Only __pushbutton_ISR.c__ is modified from part 1