// Stephen Donchez
// Dr. Wang
// ECE 8448-DL1
// 18 March 2020

/******************************************************************************
* main.c -	main routine and configuration subroutines for Homework
*			Assignment 6, part 2. See README.md for assignment description.
******************************************************************************/

#include "include/tone.h"           //defines notes
#include "intelfpgaup/audio.h"      //intel provided audio interface
#include <math.h>                   //standard c math library
#include <string.h>                 //for string length checking

#define PI (4.0 * (atan(1.0)))      //since M_PI isn't actually standardized

void play_notes(char* notes, int num_samples);           //main.c
double build_sample(int current_sample, double note);   //main.c
void write_sample(double sample);                       //main.c

/******************************************************************************
 * main: Configures audio codec and plays the tones specified in the assignment
 *       decription.
 *
 * @param	N/A
 * @returns	N/A
 * @note	N/A
 * @see		full description of program functionality in README.md
 *          Intel's "Using Linux on DE-Series Boards
 *****************************************************************************/
int main(void)
{
    int tone_length = 1000;  //miliseconds
    int num_samples = tone_length / 1000.0 * MAX_SAMPLING_RATE; //from audio.h
    char in[14];
    printf("enter a 13 bit string of ones and zeros to select the tones to be summed:\n");
    gets(in);
    int len = strlen(in);
    if (len != 13)
    {
        printf("invalid input. Expected 13 elements, %d given\n", len);
        return -1;
    }
         
    audio_open();                       //audio.h - open connection with codec
    audio_init();                       //audio.h - configure codec
    audio_rate(MAX_SAMPLING_RATE);      //audio.h - set sample rate to 48k


    //play notes for specified duration
    play_notes(in, num_samples);

    audio_close();                      //audio.h - close connection with codec  
    return 0;
} //main

/******************************************************************************
 * play_note: builds and outputs the apropriate number of samples to generate
 *            the desired note for the desired length of time
 * @param	note: the note to be played, passed by frequency, defined in tone.h
 * @param   num_samples: number of samples to generate, computed in main
 * @returns	N/A
 * @note    N/A
 * @see		N/A
 *****************************************************************************/
void play_notes(char * notes, int num_samples)
{
    double sample, note;
    for (int current_sample = 0; current_sample < num_samples; current_sample++)
    {
        sample = 0.0;
        for (int current_note = 0; current_note < 13; current_note++)
        {
            int enabled = *(notes + current_note)-48; //convert from ascii 
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
             //if something invalid got input into the terminal, abort
            if (enabled != 0 && enabled != 1)
            {
                printf("invalid value entered in position %i, 1 or 0 expected\n",
                    current_note);
                exit(-1);
            }
            note = note * enabled;
            //add the correct value for the current note to the current sample
            sample += build_sample(current_sample, note); //main.c
        } //for
        write_sample(sample);
    } //for
} //play_note

/******************************************************************************
 * build_sample: builds the current sample from the sin wave
 * @param	note: the note to be played, passed by frequency, defined in tone.h
 * @param   current_sample: the current sample in the sample set (where in the
                sin wave the sample falls
 * @returns	N/A
 * @note    N/A
 * @see		Intel's "Using Linux on DE-Series Boards
 *****************************************************************************/
double build_sample(int current_sample, double note)
{
    return ((MAX_VOLUME / 13.0) * 
        sin(note * current_sample / MAX_SAMPLING_RATE * 2 * PI));
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
    audio_wait_write();         //audio.h
    audio_write_left(sample);   //audio.h
    audio_write_right(sample);  //audio.h
} //write_sample