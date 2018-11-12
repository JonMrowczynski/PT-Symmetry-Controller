#ifndef _PENDULUM_H_
#define _PENDULUM_H_

#define DAMPENING_PHOTOGATE_PIN RC1
#define DRIVING_PHOTOGATE_PIN   RC2

#define REQUIRED_PHOTOGATE_SAMPLES 21
#define NUM_OF_DATA_POINTS ((REQUIRED_PHOTOGATE_SAMPLES - 1) / 2)

typedef struct Pendulum {
    unsigned char photogateSamples;
    unsigned short photogateTimes[REQUIRED_PHOTOGATE_SAMPLES];
    unsigned short shorterPartialPeriod;
    unsigned short longerPartialPeriod;
} Pendulum;

extern Pendulum dampeningPendulum;
extern Pendulum drivingPendulum;

extern void measurePhotogateTimes(Pendulum* dampeningPendulum, Pendulum* drivingPendulum);

extern void calculatePartialPeriods(Pendulum* pendulum);

#endif