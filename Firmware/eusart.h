#ifndef _EUSART_H_
#define _EUSART_H_

// Constants that just help with readability

#define PPSTX           0b10101
#define RC4RX           0b10100
#define ASYNCHRONOUS    0
#define SP1BRGL_INIT    51

/**
 * Initializes the EUSART such that data can be sent and received from/by the
 * microcontroller.
 */

extern void initEUSART(void);

#endif