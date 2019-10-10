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
 * @author Jon Mrowczynski
 */

#include <pic.h>
#include <stdbool.h>
#include "pins.h"
#include "configuration.h"
#include "eusart.h"
#include "SolenoidController.h"

/*
 * MIDI is used to digitally control the state of two solenoids. 
 * 
 * The first byte (or status byte) of the MIDI message should always be a 
 * channel 0 on message. Otherwise, the rest of the data is ignored. 
 * 
 * The second byte (or first data byte) of the MIDI message should represent the 
 * solenoid whose state is to be changed. If the first data byte does not 
 * represent any of the solenoids, then the rest of the data is ignored. 
 * 
 * The third and final byte (or the second data byte) represents the new state 
 * of the solenoid. If the value of the byte is 0, then power is cut from the 
 * solenoid. Any other value and power is sent to the solenoid.
 */

void __interrupt() isr(void) {
    static unsigned char statusByte     = 0;
    static unsigned char dataByte1      = 0;
    static unsigned char dataByte2      = 0;
    static unsigned char receiveCounter = 0;
    if (RCIF) {   
        ++receiveCounter;
        switch(receiveCounter) {
            case 1:
                statusByte = RCREG;
                if (statusByte != CHANNEL0_NOTE_ON) { receiveCounter = 0; }
                break;
            case 2:
                dataByte1 = RCREG;
                if (dataByte1 != DRIVING_SOLENOID_NOTE || dataByte1 != DAMPENING_SOLENOID_NOTE) { receiveCounter = 0; }
                break;
            case 3:
                dataByte2 = RCREG;
                if (dataByte1 == DRIVING_SOLENOID_NOTE) { DRIVING_SOLENOID = dataByte2; } 
                else if (dataByte1 == DAMPENING_SOLENOID_NOTE) { DAMPING_SOLENOID = dataByte2; }
                receiveCounter = 0;
                break;
            default:
                receiveCounter = 0;
                RCREG = 0;
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