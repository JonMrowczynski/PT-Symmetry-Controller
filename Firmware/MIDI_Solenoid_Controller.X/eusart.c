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
#include "eusart.h"

void initEUSART() {
    RCIE        = true;     // Enable receive interrupts for now
    RXPPS       = PPS_RA4;
    RX_PIN_MODE = DIGITAL;  
    RX_PIN      = INPUT;        
    RX9         = false;        // Disable 9-bit receptions
    BRGH        = true;         // Enable High speed baud rate
    BRG16       = true;         // Use 16-bit Baud Rate Generator
    SYNC        = ASYNCHRONOUS; // Operate in asynchronous mode
    SPEN        = true;         // Enable serial port 
    CREN        = true;         // Enable continuous reception
    SP1BRGL     = SP1BRGL_INIT;
    SP1BRGH     = SP1BRGH_INIT;
}

void clearOverrunError() {
    static unsigned char temp = 0; 
    do {                
        temp = RCREG;  
        temp = RCREG;  
        CREN = false; 
        CREN = true;  
    } while(OERR); 
}

void clearFramingError() {
    static unsigned char temp = 0;
    do {
        temp = RCREG;
        temp = RCREG;
        SPEN = false;
        SPEN = true;
    } while (FERR);
}