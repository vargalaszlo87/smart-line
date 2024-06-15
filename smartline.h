#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <stdio.h>

#ifndef SMARTLINE_H_INCLUDED
#define SMARTLINE_H_INCLUDED

#include "entity.h"

typedef struct _smartline {
    // time
    double timerIncrementum;
    double sysTime;
    int sysTick;
    pthread_t sysTimerThread;
//  pthread_mutex_t sysTimeThreadMutex;  -- later
    sem_t sysTimeThreadMutex;
    clock_t startTimer;
    clock_t endTimer;
    double totalTimer;
    // entity
    int16_t entitySize;
    int16_t* entityID;
} smartline;

static smartline* psmartline;

#endif // SMARTLINE_H_INCLUDED
