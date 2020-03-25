// Stephen Donchez
// Dr. Wang
// ECE 8448-DL1
// 25 March 2020

/******************************************************************************
* keyboard.h -	defines functions and constants for control of a USB keyboard
******************************************************************************/

#pragma once
#include <stdio.h>
#include <fcntl.h>


#define KEY_RELEASED 0
#define KEY_PRESSED 1


/******************************************************************************
 * keyboard_configure: verifies that a valid path to a device has been passed,
 *	and attempts to open it
 * @param	path: linux path to keyboard device
 * @returns	-1 if unable to open the device
 * @returns file descriptor if successful
 * @note    N/A
 * @see		N/A
 *****************************************************************************/
int keyboard_configure(char* path)
{
	int fd;							//file descriptor for keyboard device

	//verify path was passed
	if (path == NULL)
	{
		printf("specify the path to the keyboard device"\ 
			"(ex. /dev/input/by-id/HP-KEYBOARD)\n");
		return -1;
	} //if

	//attempt to open device as readonly and nonblocking
	if ((fd = open(path, O_RDONLY | O_NONBLOCK)) == -1)
	{
		//inform the user if this fails
		printf("could not open %s\n", path);
		return -1;
	} //if

	//return the file descriptor for the keyboard
	return fd;
} //keyboard_configure


/******************************************************************************
 * keyboard_close: closes the connection to the keyboard device
 * @param	N/A
 * @returns	N/A
 * @note    N/A
 * @see		N/A
 *****************************************************************************/
void keyboard_close(int fd)
{
	close(fd);
} //keyboard_close