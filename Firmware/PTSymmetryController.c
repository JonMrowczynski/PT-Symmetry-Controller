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
#include "tmr0.h"
#include "pendulum.h"
#include "PTSymmetryController.h"

void main(void) {
    
    Pendulum dampeningPendulum = {.photogateSamples = 0, .shorterPartialPeriod = 0, .longerPartialPeriod = 0};
    Pendulum drivingPendulum = {.photogateSamples = 0, .shorterPartialPeriod = 0, .longerPartialPeriod = 0};
    
    PIC_INDICATOR_LED_PIN = OFF;
    initPins();
    initTMR0();
    PIC_INDICATOR_LED_PIN = ON;
    
    printf("Firmware version: %d.%d", FIRMWARE_MAJOR_VERSION, FIRMWARE_MINOR_VERSION);
    __delay_ms(3000);
    printf("Set pendulums in motion, then press Start");
    
    while(START_PIN);
    PIC_INDICATOR_LED_PIN = OFF;
    while(!START_PIN);
    PIC_INDICATOR_LED_PIN = ON;
    printf("Running...");
    
    measurePhotogateTimes(&dampeningPendulum, &drivingPendulum);
    
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