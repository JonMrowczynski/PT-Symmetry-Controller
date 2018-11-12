#ifndef _TMR0_H_
#define _TMR0_H_

#define TIMER_16BIT                 1
#define NO_POSTSCALER_RATE          0b0000
#define INSTRUCTION_CLOCK_SOURCE    0b010
#define SYNCHRONIZED                0
#define PRESCALER_RATE_8192         0b1101

#define ENABLE_TMR0                 1
#define DISABLE_TMR0                0

#define initTMR0()                                                                          \
    T0CON0 = (TIMER_16BIT << 4) | NO_POSTSCALER_RATE;                                       \
    T0CON1 = (INSTRUCTION_CLOCK_SOURCE << 5) | (SYNCHRONIZED << 4) | PRESCALER_RATE_8192;   \
    
#define calculateTimeInMilliseconds() (((TMR0H << 8) | TMR0L) * 128) / 125

#endif