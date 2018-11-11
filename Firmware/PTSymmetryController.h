/*
 * File Name    : PTSymmetryController.h
 * Version      : 1.0
 * Author       : Jon Mrowczynski
 * Target       : PIC16F18323
 * Compiler     : XC8 v2.00 Free Version
 * IDE          : MPLAB X IDE v5.05
 * Programmer   : PICkit3
 * Last Updated : 10/16/2018
 */

#ifndef _PT_SYMMETRY_CONTROLLER_H_
#define _PT_SYMMETRY_CONTROLLER_H_

#define FIRMWARE_MAJOR_VERSION  1
#define FIRMWARE_MINOR_VERSION  0

#define PIC_INDICATOR_LED_PIN   RA5
#define START_PIN               RA4
#define DAMPEN_OUT_PIN          RA2
#define DRIVING_OUT_PIN         RC0
#define DAMPENING_PHOTOGATE_PIN RC1
#define DRIVING_PHOTOGATE_PIN   RC2

#define ON  1
#define OFF 0

#define NUM_OF_SAMPLES 21
#define NUM_OF_DATA_POINTS ((NUM_OF_SAMPLES - 1) / 2)
#define DISABLE_THIS_INTERRUPT 0

typedef struct Pendulum {
    unsigned char timeCounts;
    unsigned short photogateTimes[NUM_OF_SAMPLES];
    unsigned short shorterPartialPeriod;
    unsigned short longerPartialPeriod;
} Pendulum;

extern void calculatePartialPeriods(volatile Pendulum* pendulum);

#endif