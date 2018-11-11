/*
 * File Name    : PTSymmetryController.c
 * Version      : 1.0
 * Author       : Jon Mrowczynski
 * Target       : PIC16F18323
 * Compiler     : XC8 v2.00 Free Version
 * IDE          : MPLAB X IDE v5.05
 * Programmer   : PICkit3
 * Last Updated : 11/11/2018
 */

#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configuration.h"
#include "PTSymmetryController.h"

volatile Pendulum dampeningPendulum;
volatile Pendulum drivingPendulum;

void __interrupt() isr(void) {
    if (DAMPENING_PHOTOGATE_PIN) {
        if (dampeningPendulum.timeCounts < NUM_OF_SAMPLES)
            dampeningPendulum.photogateTimes[dampeningPendulum.timeCounts++] = TMR0;
        else 
            DISABLE_THIS_INTERRUPT;
    } else if (DRIVING_PHOTOGATE_PIN) {
        if (drivingPendulum.timeCounts < NUM_OF_SAMPLES)
            dampeningPendulum.photogateTimes[drivingPendulum.timeCounts++] = TMR2;
        else 
            DISABLE_THIS_INTERRUPT;
    }
}

void main(void) {
    
    PIC_INDICATOR_LED_PIN = OFF;
    initPins();
    PIC_INDICATOR_LED_PIN = ON;
    
    printf("Firmware version: %d.%d", FIRMWARE_MAJOR_VERSION, FIRMWARE_MINOR_VERSION);
    __delay_ms(3000);
    printf("Set pendulums in motion, then press Start");
    
    while(START_PIN);
    PIC_INDICATOR_LED_PIN = OFF;
    while(!START_PIN);
    PIC_INDICATOR_LED_PIN = ON;
    printf("Running...");

    while(dampeningPendulum.timeCounts < NUM_OF_SAMPLES && drivingPendulum.timeCounts < NUM_OF_SAMPLES);
    
    calculatePartialPeriods(&dampeningPendulum);
    calculatePartialPeriods(&drivingPendulum);
    
    while(true) {
        if (DAMPENING_PHOTOGATE_PIN) {
            
        }
        if (DRIVING_PHOTOGATE_PIN) {
            
        }
    }
    
    PIC_INDICATOR_LED_PIN = OFF;
    
    return;
}

void calculatePartialPeriods(volatile Pendulum* pendulum) {
    unsigned long total1 = 0;
    unsigned long total2 = 0;
    for (unsigned char i = 0; i < NUM_OF_DATA_POINTS; i = 2 * (i + 1)) {
        total1 += pendulum->photogateTimes[i + 1] - pendulum->photogateTimes[i];
        total2 += pendulum->photogateTimes[i + 2] - pendulum->photogateTimes[i + 1];
    }
    unsigned short avg1 = total1 / NUM_OF_DATA_POINTS;
    unsigned short avg2 = total2 / NUM_OF_DATA_POINTS;
    pendulum->shorterPartialPeriod = avg2 > avg1 ? avg1 : avg2;
    pendulum->longerPartialPeriod = avg1 > avg2 ? avg1 : avg2;
}
