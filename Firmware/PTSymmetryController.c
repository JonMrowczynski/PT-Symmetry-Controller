/*
 * File Name    : PTSymmetryController.c
 * Version      : 1.0
 * Author       : Jon Mrowczynski
 * Target       : PIC16F18323
 * Compiler     : XC8 v2.00 Free Version
 * IDE          : MPLAB X IDE v5.05
 * Programmer   : PICkit3
 * Last Updated : 10/16/2018
 */


#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configuration.h"
#include "PTSymmetryController.h"

void __interrupt() isr(void) {
    
}

void main(void) {
    
    PIC_INDICATOR_LED_PIN = OFF;
    initPins();
    PIC_INDICATOR_LED_PIN = ON;
    
    printf("Firmware version: %d.%d", FIRMWARE_MAJOR_VERSION, FIRMWARE_MINOR_VERSION);
    __delay_ms(3000);
    printf("Set the pendulums in motion, then press Start");
    
    while(START_PIN);
    PIC_INDICATOR_LED_PIN = OFF;
    while(!START_PIN);
    PIC_INDICATOR_LED_PIN = ON;
    printf("Running...");
    
    while(true) {
        if (DAMPENING_PHOTOGATE_PIN) {
            
        }
        if (DRIVING_PHOTOGATE_PIN) {
            
        }
    }
    
    PIC_INDICATOR_LED_PIN = OFF;
    
    return;
}
