#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#ifndef SMARTLINE_H_INCLUDED
#define SMARTLINE_H_INCLUDED

#include "entity.h"

typedef struct _smartline {
    // time
    double timerIncrementum;
    double timerDivider;
    double sysTime;
    int sysTick;
    pthread_t sysTimerThread;
//  pthread_mutex_t sysTimeThreadMutex;  -- later
//  sem_t sysTimeThreadMutex;
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

#ifdef __linux__
void sysTimerHandler(int , siginfo_t *, void *);
#elif _WIN32 || _WIN64
void sysTimerHandler();
#endif
void* sysTimer(void *);
bool sysTimerStart(smartline *);
void smartLineInit(smartline *);
bool smartLineMake(smartline *);

#endif // SMARTLINE_H_INCLUDED
