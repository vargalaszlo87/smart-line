#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include "entity.h"

#ifndef SMARTLINE_H_INCLUDED
#define SMARTLINE_H_INCLUDED

//pthread_mutex_t lock;

typedef struct _smartline {
    // mutex
        pthread_mutex_t lock;
    // time
        double timerIncrementum;
        double timerDivider;
        double sysTime;
        double sendTime;
        double sendTimer;
        double takeTime;
        double takeTimer;
        int sysTick;
        #ifdef __linux__
        timer_t timerid;
        struct sigevent sev;
        struct itimerspec its;
        struct sigaction sa;
        #endif
    // entity
        uint16_t entityCapacity;
        uint16_t entitySize;
        uint16_t* entityID;
        void** entityPointer;
} smartline;

static smartline* psmartline;

void* sysTimerHandler(void *);
void* sysMaterialHandler(void* );
bool sysTimerStart(smartline *);
bool smartLineInit(smartline *);


#endif // SMARTLINE_H_INCLUDED
