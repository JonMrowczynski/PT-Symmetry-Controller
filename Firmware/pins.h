#ifndef _PINS_H_
#define _PINS_H_

#define OUTPUT  0
#define INPUT   1

#define ANALOG_INPUT    1
#define DIGITAL_PIN     0

#define ON  1
#define OFF 0

#define PIC_INDICATOR_LED_PIN   RA5
#define START_PIN               RA4
#define DAMPENING_SOLENOID_PIN  RA2
#define DRIVING_SOLENOID_PIN    RC0

#define initPins()      \
    TRISA = 0b00011011; \
    TRISC = 0b00000110

#endif