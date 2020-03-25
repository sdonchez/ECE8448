// Stephen Donchez
// Dr. Wang
// ECE 8448-DL1
// 25 March 2020

/******************************************************************************
* main.c -	main routine and configuration subroutines for Homework
*			Assignment 6, part 3. See README.md for assignment description.
******************************************************************************/

#include "include/tone.h"           //defines notes
#include "intelfpgaup/audio.h"      //intel provided audio interface
#include <math.h>                   //standard c math library
#include <string.h>                 //for string length checking
#include <stdio.h>                  //printf
#include <stdlib.h>                 //exit
#include <pthread.h>                //for threading
#include "include/keyboard.h"       //to configure keyboard
#include <linux/input.h>            //to read from keyboard


#define PI (4.0 * (atan(1.0)))      //since M_PI isn't actually standardized

/******************************************************************************
 * play_note: builds and outputs the correct combination of notes for the
 *            current sample
 *****************************************************************************/
void play_notes(int tone_volume[], int num_samples);           

/******************************************************************************
 * build_sample: builds the current sample from the sin wave
 *****************************************************************************/
double build_sample(double note, int current_sample, int volume);

/******************************************************************************
 * write_sample: writes the current sample to the audio device
 *****************************************************************************/
void write_sample(double sample);

/******************************************************************************
 * audio_thread: seperate thread responsible for audio tasks. Checks to see if
 *               the program is exiting, then plays the next sample before
 *               decrementing the volume array to achieve a fade out that
 *               mimics a real piano.
 *****************************************************************************/
void* audio_thread();

/******************************************************************************
 * decrement_volume: locks the volume mutex, decrements all note volumes by a
 *                   scale factor, then unlocks the mutex to allow for new
 *                   keystrokes to be recorded
 *****************************************************************************/
void decrement_volume(int tone_volume[], double scale_factor);

int tone_volume[13];               //global array to store volume for each note

//mutex to avoid race conditions since both threads access tone_volume
pthread_mutex_t mutex_tone_volume; 

/******************************************************************************
 * main: Configures audio codec and the keyboard input, spawns the seperate
 *  audio thread, and then enters a loop of waiting for keyboard input and
 *  resetting the volume level for that note to the maximum level. Upon 
 *  termination via the "X" key on the keyboard, cancels the audio thread and
 *  waits for it to exit before closing the audio and keyboard interfaces and
 *  returning.
 *
 * @param	argc: number of arguments, including the executable name, passed
 *          from the command line. 2 expected.
 * @param   argv: the text of the arguments. argv[0] is the executable name,
 *          argv[1] is the path to the keyboard device
 * @returns	N/A
 * @note	N/A
 * @see		full description of program functionality in README.md
 *          Intel's "Using Linux on DE-Series Boards
 *****************************************************************************/
int main(int argc, char *argv[])
{
    int err;                            //error variable for thread creation
    pthread_t tid;                      //thread id for the audio thread
    int fd;                             //file descriptor for the keyboard

    audio_open();                       //audio.h - open connection with codec
    audio_init();                       //audio.h - configure codec
    audio_rate(MAX_SAMPLING_RATE);      //audio.h - set sample rate to 48k

    //establish keyboard configuration
    if ((fd = keyboard_configure(argv[1])) == -1)   //keyboard.h
    {
        printf("keyboard configuration failed, program will terminate\n");
        return -1;
    } //if

    if ((err = pthread_create(&tid, NULL, &audio_thread, NULL)) != 0)
        printf("pthread_create failed:[%s]\n", strerror(err));

    struct input_event ev;              //stores key press information
    int event_size = sizeof(struct input_event);

    int exit = 0;                       //loop exit flag for safe termination

    while (!exit)
    {

        //if no key is pressed, return to top of loop
        if (read(fd, &ev, event_size) < event_size)
        {
            //no input
            continue;
        } //if

        //if a key is pressed, see what it is and act accordingly
        if (ev.type == EV_KEY && ev.value == KEY_PRESSED)
        {
            //lock the volume mutex to prevent race conditions
            pthread_mutex_lock (&mutex_tone_volume);
            int key = (int)ev.code;
            switch (key)
            {
            //if it's a valid key, reset the volume of that note to the max
             //volume (divided by 13 to prevent overflows from summing keys)
            case KEY_Q:
                tone_volume[0] = (MAX_VOLUME / 13);
                break;
            case KEY_2:
                tone_volume[1] = (MAX_VOLUME / 13);
                break;
            case KEY_W:
                tone_volume[2] = (MAX_VOLUME / 13);
                break;
            case KEY_3:
                tone_volume[3] = (MAX_VOLUME / 13);
                break;
            case KEY_E:
                tone_volume[4] = (MAX_VOLUME / 13);
                break;
            case KEY_R:
                tone_volume[5] = (MAX_VOLUME / 13);
                break;
            case KEY_5:
                tone_volume[6] = (MAX_VOLUME / 13);
                break;
            case KEY_T:
                tone_volume[7] = (MAX_VOLUME / 13);
                break;
            case KEY_6:
                tone_volume[8] = (MAX_VOLUME / 13);
                break;
            case KEY_Y:
                tone_volume[9] = (MAX_VOLUME / 13);
                break;
            case KEY_7:
                tone_volume[10] = (MAX_VOLUME / 13);
                break;
            case KEY_U:
                tone_volume[11] = (MAX_VOLUME / 13);
                break;
            case KEY_I:
                tone_volume[12] = (MAX_VOLUME / 13);
                break;
            //if X is pressed, exit the loop so the program can close cleanly
            case KEY_X:
                printf("X pressed, application terminating\n");
                exit = 1;
                break;
             //if something else is pressed, tell the user
            default:
                printf("Invalid key pressed, please try again\n");
            }

            //unlock the mutex so that the audio thread can use it
            pthread_mutex_unlock(&mutex_tone_volume);
        }
    }

    //if the loop is exited, program is terminating. Ask audio thread to close.
    pthread_cancel(tid);
    pthread_join(tid, NULL);

    //close keyboard and audio interfaces
    keyboard_close(fd);                 //keyboard.h

    audio_close();                      //audio.h - close connection with codec  
    return 0;
} //main

/******************************************************************************
 * play_note: builds and outputs the correct combination of notes for the
 *            current sample
 * @param	tone_volume[]: 13 int array of volumes, one per note
 * @param   current_samples: the current sample number - for the sin function
 * @returns	N/A
 * @note    N/A
 * @see		main, decrement_volume for tone_volume interaction
 *****************************************************************************/
void play_notes(int tone_volume[], int current_sample)
{
    double sample, note;                    //note: frequency to be outputted
        sample = 0.0;                       //running value of current sample
        for (int current_note = 0; current_note < 13; current_note++)
        {
            //set the volume according to what note is being summed
            int volume = *(tone_volume+current_note);
            switch (current_note) //determine the apropriate frequency
            {
            case 0:
                note = LOWERC;
                break;
            case 1:
                note = CSHARP;
                break;
            case 2:
                note = D;
                break;
            case 3:
                note = DSHARP;
                break;
            case 4:
                note = E;
                break;
            case 5:
                note = F;
                break;
            case 6:
                note = FSHARP;
                break;
            case 7:
                note = G;
                break;
            case 8:
                note = GSHARP;
                break;
            case 9:
                note = A;
                break;
            case 10:
                note = ASHARP;
                break;
            case 11:
                note = B;
                break;
            default:
                note = UPPERC;
                break;
            } //switch
            //add the correct value for the current note to the current sample
            sample += build_sample(current_sample, note, volume); //main.c
        } //for
        //write the sample to the output
        write_sample(sample);
} //play_note

/******************************************************************************
 * build_sample: builds the current sample from the sin wave
 * @param	note: the note to be played, passed by frequency, defined in tone.h
 * @param   current_sample: the current sample in the sample set (where in the
 *              sin wave the sample falls)
 * @param   volume: the volume of the current sample, as a scalar for the sin
 * @returns	N/A
 * @note    N/A
 * @see		Intel's "Using Linux on DE-Series Boards
 *****************************************************************************/
double build_sample(double note, int current_sample,  int volume)
{
    //convert Hz to rads and determine appropriate sample value
    return (volume * sin(note * current_sample / MAX_SAMPLING_RATE * 2 * PI));
} //build_sample

/******************************************************************************
 * write_sample: writes the current sample to the audio device
 * @param	sample: the current sample as generated by build_sample
 * @returns	N/A
 * @note    N/A
 * @see		Intel's "Using Linux on DE-Series Boards
 *****************************************************************************/
void write_sample(double sample)
{
    //wait for fifos to be ready for input, then fill them
    audio_wait_write();         //audio.h
    audio_write_left(sample);   //audio.h
    audio_write_right(sample);  //audio.h
} //write_sample

/******************************************************************************
 * audio_thread: seperate thread responsible for audio tasks. Checks to see if
 *               the program is exiting, then plays the next sample before
 *               decrementing the volume array to achieve a fade out that
 *               mimics a real piano.
 * @param	N/A
 * @returns	N/A
 * @note    N/A
 * @see		main for thread lifecycle
 *****************************************************************************/
void* audio_thread()
{
    int current_sample = 0;             //counter to allow sine wave to build
    while (1)
    {  
        pthread_testcancel();           //verify program isn't exiting
        play_notes(tone_volume, current_sample++);
        decrement_volume(tone_volume, .9999);
    } //while
}   //audio_thread

/******************************************************************************
 * decrement_volume: locks the volume mutex, decrements all note volumes by a 
 *                   scale factor, then unlocks the mutex to allow for new
 *                   keystrokes to be recorded
 * @param	tone_volume: 13 int array of volumes for each key (by reference)
 * @param   scale_factor: scalar to decrement volume by. Should be very close
 *          to 1 due to exponential nature of process and high sample rate.
 * @returns	N/A
 * @note    N/A
 * @see		main for other mutex access
 *****************************************************************************/
void decrement_volume(int tone_volume[], double scale_factor)
{
    pthread_mutex_lock(&mutex_tone_volume);

    //apply the same scalar to all keys
    for (int current_note = 0; current_note < 13; current_note++)
    {
        tone_volume[current_note] *= scale_factor;
    } //for
    pthread_mutex_unlock(&mutex_tone_volume);
} //decrement_volume
