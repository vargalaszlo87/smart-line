#include <pthread.h>
#include <time.h>
#include <stdio.h>

#ifndef SMARTLINE_H_INCLUDED
#define SMARTLINE_H_INCLUDED

#include "entity.h"

typedef struct _smartline {
    // time
    double timeIncrementum;
    double sysTime;
    double sysTick;
    pthread_t sysTimerThread;
    clock_t startTimer;
    clock_t endTimer;
    // entity
    int16_t entitySize;
    int16_t* entityID;
} smartline;

static smartline* psmartline;

#endif // SMARTLINE_H_INCLUDED
