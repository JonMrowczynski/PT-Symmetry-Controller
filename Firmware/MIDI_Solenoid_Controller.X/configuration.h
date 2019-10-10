/*
 * Author: Jon Mrowczynski
 * 
 * This file sets the configuration registers of the PIC.
 */

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

// Configuration Word 1: Oscillators

#pragma config FCMEN    = OFF       // Disable Fail-safe clock monitor. Only using internal 32MHz clock
#pragma config CSWEN    = OFF       // Disable clock switching. Only using internal 32MHz clock
#pragma config CLKOUTEN = OFF       // Disable clock out function
#pragma config RSTOSC   = HFINT32   // Use 32MHz internal oscillator
#pragma config FEXTOSC  = OFF       // Disable use of an external clock

// Configuration Word 2: Supervisors

#pragma config DEBUG    = OFF   // Disable background debugger.
#pragma config STVREN   = ON    // Enable stack overflow/underflow reset
#pragma config PPS1WAY  = ON    // Lock peripheral pin select (PPS) registers after one clear/set cycle
#pragma config BORV     = HIGH  // Set High (2.7V) Brown-out reset
#pragma config BOREN    = ON    // Enable Brown-out reset
#pragma config LPBOREN  = OFF   // Disable low-power brown-out reset
#pragma config WDTE     = OFF   // Disable watchdog timer
#pragma config PWRTE    = ON    // Enable power-up timer
#pragma config MCLRE    = ON    // Enable master clear functionality

// Configuration Word 3: Memory

#pragma config LVP  = ON    // Enable low-voltage programming
#pragma config WRT  = ALL   // Protect all program memory from self-write since not implementing bootloader

// Configuration Word 4: Code Protection 

#pragma config CPD  = ON    // Enable Data EEPROM code protection 
#pragma config CP   = ON    // Enable program memory code protection 

// Define the frequency of oscillation

#define _XTAL_FREQ  32000000    // Using 32MHz internal clock 

/*
 * Locks the PPS registers so that their function cannot be inadvertently 
 * changed
 */

#define LOCKPPS()       \
    do {                \
        PPSLOCK = 0x55; \
        PPSLOCK = 0xAA; \
        PPSLOCK = 0x01; \
    } while(0)

#endif