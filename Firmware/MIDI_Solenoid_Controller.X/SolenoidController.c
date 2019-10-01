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
 * This firmware allows a PIC16F628A microcontroller to control up to six servo 
 * motors and two LEDs in parallel. TIMER2 match interrupts are used to create 
 * the software PWM signals with a period of 200ms that are necessary to control
 * up to six servo motors.
 * 
 * For the Tower Pro 9g servos, a full counterclockwise rotation requires a 
 * pulse width of 2.0ms, a full clockwise rotation requires a pulse width of 
 * 1.0ms, and the central or neutral position requires a pulse width of 1.5ms.
 * 
 * MIDI is used as the communication protocol to control the servo motors. The 
 * velocity of the MIDI note is used to determine the angular position of a 
 * corresponding servo motor arm.
 * 
 * The notes that are associated with the motors and the lights match the tones 
 * of a C4 pentatonic scale, which are C4, D4, E4, G4, A4, C5, and D5.
 * 
 * To learn more information about how each module is specifically implemented,
 * check out the corresponding header file.
 * 
 * @author Jon Mrowczynski
 */

#include <pic.h>
#include <stdbool.h>
#include "configuration.h"
#include "usart.h"
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
                // use data
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
    
    initUSART();
    
    PEIE = true;    // Enable peripheral interrupts
    GIE = true;     // Enable global interrupts

    /* 
     * If the USART experiences an overrun error and/or a framing error, fix it.
     * Otherwise, wait to perform the interrupt service routine.
     */
    
    while(true) {
        if (OERR) { clearOverrunError(); }
        if (FERR)  { clearFramingError(); }
    }
}