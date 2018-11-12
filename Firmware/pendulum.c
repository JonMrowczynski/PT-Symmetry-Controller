#include <xc.h>
#include "tmr0.h"
#include "pendulum.h"

void measurePhotogateTimes(Pendulum* dampeningPendulum, Pendulum* drivingPendulum) {
    T0EN = ENABLE_TMR0;
    while(dampeningPendulum->photogateSamples < REQUIRED_PHOTOGATE_SAMPLES && drivingPendulum->photogateSamples < REQUIRED_PHOTOGATE_SAMPLES) {
        if (!DAMPENING_PHOTOGATE_PIN) {
            if (dampeningPendulum->photogateSamples < REQUIRED_PHOTOGATE_SAMPLES)
                dampeningPendulum->photogateTimes[dampeningPendulum->photogateSamples++] = calculateTimeInMilliseconds();
        } else if (!DRIVING_PHOTOGATE_PIN) {
            if (drivingPendulum->photogateSamples < REQUIRED_PHOTOGATE_SAMPLES)
                dampeningPendulum->photogateTimes[drivingPendulum->photogateSamples++] = calculateTimeInMilliseconds();
        }
    }
    T0EN = DISABLE_TMR0;
    TMR0H = TMR0L = 0;
}

void calculatePartialPeriods(Pendulum* pendulum) {
    unsigned long total1 = 0;
    unsigned long total2 = 0;
    for (unsigned char i = 0; i < NUM_OF_DATA_POINTS; i = 2 * (i + 1)) {
        total1 += pendulum->photogateTimes[i + 1] - pendulum->photogateTimes[i];
        total2 += pendulum->photogateTimes[i + 2] - pendulum->photogateTimes[i + 1];
    }
    unsigned short avg1 = total1 / NUM_OF_DATA_POINTS;
    unsigned short avg2 = total2 / NUM_OF_DATA_POINTS;
    pendulum->shorterPartialPeriod = avg2 > avg1 ? avg1 : avg2;
    pendulum->longerPartialPeriod = avg1 > avg2 ? avg1 : avg2;
}
