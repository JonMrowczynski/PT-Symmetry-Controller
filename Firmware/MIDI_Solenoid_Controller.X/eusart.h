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
 * A baud rate (BRATE) of 31250 is needed to receive MIDI messages. To calculate
 * the SPBRG register value to generate that baud rate, the following formula, 
 * which only holds for high baud rates (BRGH == 1) is used:
 *
 * SPBRG = ( Fosc / ( 16 * Desired Baud Rate ) ) - 1
 *
 * Where Fosc is the frequency of the clock. In our case, with an external 
 * oscillator of 20MHz, the equation would yield:
 *
 * SPBRG = 20,000,000 / ( 16 * 31250 ) - 1 = 39
 * 
 * Note that that the theoretical baud rate error is 0%. However, There is going
 * to be some error introduced from error in the frequency of the quartz crystal 
 * resonator.
 * 
 * @author Jon Mrowczynski
 */

#ifndef _EUSART_H_
#define _EUSART_H_

#include "configuration.h"

#define BAUD_RATE   31250

// Constants that just help with readability

#define ASYNCHRONOUS    0
#define SP1BRGL_INIT    ((unsigned char) (((_XTAL_FREQ / (4L * BAUD_RATE)) - 1) & 0xFF))
#define SP1BRGH_INIT    ((unsigned char) ((((_XTAL_FREQ / (4L * BAUD_RATE)) - 1) >> 8) & 0xFF))

/**
 * Initializes the EUSART module such that data can be sent and received from/by
 * the PIC.
 */

extern void initEUSART(void);

/**
 * In case of a receive register overrun error, reset the EUSART to be able to
 * receive messages again. This should be called in any loop where it is 
 * required for the EUSART to be able to receive data in order to exit the loop.
 */

extern void clearOverrunError(void);

/**
 * A framing error can occur if there is any noise on the signal lines, or if 
 * the baud rates of the communicating devices are different. Of course, there 
 * could potentially be a sufficient amount of noise on the signal lines caused
 * by a variety of factors. Also, the MIDI baud rate is not going to be exactly 
 * the same as the baud rate of the PIC.
 */

extern void clearFramingError(void);

#endif