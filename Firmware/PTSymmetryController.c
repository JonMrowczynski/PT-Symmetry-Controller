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
#include <stdbool.h>
#include "configuration.h"
#include "PTSymmetryController.h"

void main(void) {
    
    PIC_INDICATOR_LED = OFF;
    
    initPins();
    
    PIC_INDICATOR_LED = ON;
    
    while(true) {
        if (DAMPENING_PHOTOGATE) {
            
        }
        if (DRIVING_PHOTOGATE) {
            
        }
    }
    
    PIC_INDICATOR_LED = OFF;
    
    return;
}
