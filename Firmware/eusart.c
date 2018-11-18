#include <xc.h>
#include <stdbool.h>
#include "eusart.h"
#include "pins.h"

void initEUSART() {
    RCIE    = false;        // Disable receive interrupts
    TXIE    = false;        // Disable transmit interrupts
    RXPPS   = RC4RX;        // Make RC3 the RX pin
    RC5PPS  = PPSTX;        // Make RC4 the TX pin
    ANSC4   = DIGITAL_PIN;  // RC3 should be a digital pin
    ANSC5   = DIGITAL_PIN;  // RC4 should be a digital pin
    TRISC4  = INPUT;       
    TRISC5  = OUTPUT;
    RX9     = false;        // Disable 9-bit receptions
    TX9     = false;        // Disable 9-bit transmissions
    BRGH    = false;        // Disable High speed baud rate
    SYNC    = ASYNCHRONOUS; // Operate in asynchronous mode
    TXEN    = true;         // Enable transmissions
    SPEN    = true;         // Enable serial port 
    CREN    = false;        // Disable continuous reception
    SP1BRGL = SP1BRGL_INIT;    
}