/*
 * MIT License
 *
 * Copyright (c) 2015-2019 Jon Mrowczynski
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Contains constants that are used to handle setting the states of pins.
 * 
 * @author Jon Mrowczynski 
 */

#ifndef _PINS_H_
#define _PINS_H_

#define DIGITAL 0
#define ANALOG  1

#define OUTPUT  0
#define INPUT   1

#define DAMPENING_SOLENOID_PIN_MODE ANSA2
#define RX_PIN_MODE                 ANSA4
#define POWER_INDICATOR_PIN_MODE    ANSA5
#define DRIVING_SOLENOID_PIN_MODE   ANSC0

#define DAMPENING_SOLENOID_PIN  TRISA2
#define RX_PIN                  TRISA4
#define POWER_INDICATOR_PIN     TRISA5
#define DRIVING_SOLENOID_PIN    TRISC0

#define DAMPING_SOLENOID    LATA2
#define POWER_INDICATOR     LATA5
#define DRIVING_SOLENOID    LATC0

#define initPins()                                  \
    do {                                            \
        DAMPENING_SOLENOID_PIN_MODE = DIGITAL;      \
        RX_PIN_MODE                 = DIGITAL;      \
        POWER_INDICATOR_PIN_MODE    = DIGITAL;      \
        DRIVING_SOLENOID_PIN_MODE   = DIGITAL;      \
                                                    \
        DAMPENING_SOLENOID_PIN      = OUTPUT;       \
        RX_PIN                      = INPUT;        \
        POWER_INDICATOR_PIN         = OUTPUT;       \
        DRIVING_SOLENOID_PIN        = OUTPUT;       \
    } while(0);

typedef enum PPS_INPUTS {
    PPS_RA0, PPS_RA1, PPS_RA2, PPS_RA3, PPS_RA4, PPS_RA5, PPS_RC0 = 0b10000, 
    PPS_RC1, PPS_RC2, PPS_RC3, PPS_RC4, PPS_RC5, PPS_RC6, PPS_RC7
} PPS_INPUT;

#endif