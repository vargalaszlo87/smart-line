#include "smartline.h"


void* sysTimerHandler(void *arg) {
    // args
    smartline* sysTemp = (smartline*)arg;
    // threading
    pthread_mutex_lock(&sysTemp->lock);
    double incrementum = sysTemp -> timerIncrementum;
    double divider = sysTemp -> timerDivider;
    pthread_mutex_unlock(&sysTemp->lock);
    // timer for system
    struct timespec ts;
    ts.tv_sec = 0;
    long int nsec = (long int)((incrementum * divider) * 1e9);
    ts.tv_nsec = (long int)((incrementum * divider) * 1e9);
    long time_spent;
    // timer for process
    sysTemp->sendTimer = 0.0;
    sysTemp->takeTimer = 0.0;
    while (true) {
        clock_t begin = clock();
        // job
        pthread_mutex_lock(&sysTemp->lock);
        sysTemp->sysTick = !sysTemp->sysTick;
        sysTemp->sysTime += 0.1;
        pthread_mutex_unlock(&sysTemp->lock);
        #ifdef NDEBUG
            printf ("sysTick: %d \n",p->sysTick);
            printf ("sysTime: %lf \n",p->sysTime);
        #endif
        // process
        process01(sysTemp);     // entityJob
        process02(sysTemp);     // taker/sender

        //for (int i = 0 ; i < 1e5 ; i++) {
        //    int j = i;
        //}

        // timing
        clock_t end = clock();
        time_spent = (long int)((double)(end - begin) / CLOCKS_PER_SEC * 1e9);
        //ts.tv_nsec = (long int)(nsec - 1);

        //printf ("\n\n%d", ts.tv_nsec - time_spent);


        nanosleep(&ts, NULL);
    }
}

// DEV
bool sysTimerStart(smartline *this) {


    #ifdef __linux__
    // jelkezelo beallitasa
    this->sa.sa_flags = SA_SIGINFO;
    this->sa.sa_sigaction = sysHandler;
    sigemptyset(&this->sa.sa_mask);
    if (sigaction(SIGRTMIN, &this->sa, NULL) == -1) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with sigaction --> smartLineMake");
        #endif
        return false;
    }
    // Timer letrehozasa
    this->sev.sigev_notify = SIGEV_SIGNAL;
    this->sev.sigev_signo = SIGRTMIN;
    this->sev.sigev_value.sival_ptr = &this->timerid;
    if (timer_create(CLOCK_REALTIME, &this->sev, &this->timerid) == -1) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with timer_create --> smartLineMake");
        #endif
        return false;
    }
    // Timer beallitasa
    this->its.it_value.tv_sec = 0;
    this->its.it_value.tv_nsec = this->timerIncrementum * this->timerDivider * 1e9; // 0.1^9ns
    this->its.it_interval.tv_sec = 0;
    this->its.it_interval.tv_nsec = this->timerIncrementum * this->timerDivider * 1e9; // 0.1^9ns
    if (timer_settime(this -> timerid, 0, &this->its, NULL) == -1) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with timer_settime --> smartLineMake");
        #endif
        return false;
    }
    #elif _WIN32 || _WIN64
/*    if (pthread_create(&this->sysTimerThread, NULL, sysTimer, (void*)this) != 0) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with sysTimer-thread. (pthread_create --> smartLineMake)");
        #endif
        message("smartLineMake_DEFAULT_ERROR");
        return false;
    }
    //else
*/    //    pthread_join(this->sysTimerThread, NULL);
    #endif

   return true;
}

bool smartLineInit(smartline *s) {
    // static variable
    psmartline = s;
    // sys variables
    s->sysTick = 0;
    s->sysTime = 0.0;
    // entity array
    s->entitySize = 0;
    s->entityCapacity = 2;
    s->entityPointer = (void*)calloc(s->entityCapacity, sizeof(void));
    s->entityID = (uint16_t*)calloc(s->entityCapacity, sizeof(uint16_t));
    if (!s->entityPointer || !s->entityID) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with SmartLineInit method (Allocation)");
        #endif
        message("smartLineInit_DEFAULT_ERROR");
        return false;
    }
    if (pthread_mutex_init(&s->lock, NULL) != 0) {
        #ifdef NDEBUG
            fprintf (stderr, "> Problem with SmartLineInit method (mutex_init)");
        #endif
        message ("smartLineInit_DEFAUL_ERROR");
        return false;
    }
    return true;
}

/*void* sysMaterialHandler(void* arg) {
    smartline* sysMHTemp = (smartline*)arg;
    materialHandler();
}*/

