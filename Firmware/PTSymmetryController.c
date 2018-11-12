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
    
    // Make sure that the LED that indicates that the PIC is ready to go is turned off.
    
    PIC_INDICATOR_LED_PIN = OFF;
    
    // Variable Initializations

    Pendulum dampeningPendulum = {.photogateSamples = 0, .shorterPartialPeriod = 0, .longerPartialPeriod = 0};
    Pendulum drivingPendulum = {.photogateSamples = 0, .shorterPartialPeriod = 0, .longerPartialPeriod = 0};
    
    // Initialize all modules
    
    initPins();
    initTMR0();
    
    // Indicate that the PIC is operational and properly setup
    
    PIC_INDICATOR_LED_PIN = ON;
    
    // Display firmware version and prompt user
    
    printf("Firmware v: %d.%d", FIRMWARE_MAJOR_VERSION, FIRMWARE_MINOR_VERSION);
    __delay_ms(3000);
    printf("Swing Pendulums");
    printf("Then press Start");
    
    // Wait for the user to start the PT-symmetry controller
    
    while(START_PIN);
    PIC_INDICATOR_LED_PIN = OFF;
    while(!START_PIN);
    PIC_INDICATOR_LED_PIN = ON;
    printf("Running...");
    
    // Acquire photogate timings when each pendulum blocks its corresponding photogate
    
    measurePhotogateTimes(&dampeningPendulum, &drivingPendulum);
    
    // Calculate the asymmetric partial periods of both of the pendulums based on the photogate timings
    
    calculatePartialPeriods(&dampeningPendulum);
    calculatePartialPeriods(&drivingPendulum);
    
    // Pulse the solenoids based on the pendulums' asymmetric partial periods
    
    while(true) {
        if (DAMPENING_PHOTOGATE_PIN) {
            
        }
        if (DRIVING_PHOTOGATE_PIN) {
            
        }
    }
    
    PIC_INDICATOR_LED_PIN = OFF;
    
    return;
}