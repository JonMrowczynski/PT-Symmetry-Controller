/*
 * File Name    : configuration.h
 * Version      : 1.0
 * Author       : Jon Mrowczynski
 * Target       : PIC16F18323
 * Compiler     : XC8 v2.00 Free Version
 * IDE          : MPLAB X IDE v5.05
 * Programmer   : PICkit3
 * Last Updated : 10/16/2018
 */

#ifndef _CONFIGURATION_H_
#define _CONFIGURATION_H_

// CONFIGURATION WORD 1: OSCILLATORS

#pragma config FCMEN    = OFF
#pragma config CSWEN    = OFF
#pragma config RSTOSC   = HFINT32
#pragma config FEXTOSC  = HS

// CONFIGURATION WORD 2: SUPERVISORS

#pragma config DEBUG    = OFF
#pragma config STVREN   = ON
#pragma config PPS1WAY  = ON
#pragma config BORV     = HIGH
#pragma config BOREN    = ON
#pragma config LPBOREN  = OFF
#pragma config WDTE     = OFF
#pragma config PWRTE    = ON
#pragma config MCLRE    = ON

// CONFIGURATION WORD 3: MEMORY

#pragma config LVP  = ON
#pragma config WRT  = ALL

// CONFIGURATION WORD 4: CODE PROTECTION

#pragma config CPD  = ON
#pragma config CP   = ON

#define _XTAL_FREQ 32000000

#endif