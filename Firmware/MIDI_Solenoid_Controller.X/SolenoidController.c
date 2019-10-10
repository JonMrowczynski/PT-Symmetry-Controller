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
 * MIDI is used as the communication protocol to control the solenoids. The 
 * velocity of the MIDI note is used to determine the angular position of a 
 * corresponding servo motor arm.
 * 
 * @author Jon Mrowczynski
 */

#include <pic.h>
#include <stdbool.h>
#include "pins.h"
#include "configuration.h"
#include "eusart.h"
#include "SolenoidController.h"

void __interrupt() isr(void) {
    static unsigned char statusByte     = CLEAR;
    static unsigned char dataByte1      = CLEAR;
    static unsigned char dataByte2      = CLEAR;
    static unsigned char receiveCounter = CLEAR;
    if (RCIF) {   
        ++receiveCounter;
        switch(receiveCounter) {
            case 1:
                statusByte = RCREG;
                if (statusByte != CHANNEL0_NOTE_ON) { receiveCounter = CLEAR; }
                break;
            case 2:
                dataByte1 = RCREG;
                if (dataByte1 != DRIVEN_PENDULUM_NOTE || dataByte1 != DAMPENED_PENDULUM_NOTE) { receiveCounter = CLEAR; }
                break;
            case 3:
                dataByte2 = RCREG;
                if (dataByte1 == DRIVEN_PENDULUM_NOTE) { DRIVE_PENDULUM = dataByte2; } 
                else if (dataByte1 == DAMPENED_PENDULUM_NOTE) { DAMPEN_PENDULUM = dataByte2; }
                receiveCounter = CLEAR;
                break;
            default:
                receiveCounter = CLEAR;
                RCREG = CLEAR;
                break;
        }
    }
}

void main(void) {
    
    // Initialize everything
    
    initPins();
    initEUSART();
    LOCKPPS();
    
    PEIE = true;    // Enable peripheral interrupts
    GIE = true;     // Enable global interrupts

    /* 
     * If the EUSART experiences an overrun error and/or a framing error, fix 
     * it. Otherwise, wait to perform the interrupt service routine.
     */
    
    while(true) {
        if (OERR) { clearOverrunError(); }
        if (FERR) { clearFramingError(); }
    }
}