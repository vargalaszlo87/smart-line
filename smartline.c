#include "smartline.h"

#define NDEBUG

#ifdef __linux__
void sysTimerHandler(int signal, siginfo_t *si, void *uc)
#elif _WIN32 || _WIN64
void sysTimerHandler() {
#endif // __linux__
        smartline* p = &psmartline;
        p->sysTick = !p->sysTick;
        p->sysTime += 0.1;
        #ifdef NDEBUG
            printf ("sysTick: %d \n",p->sysTick);
            printf ("sysTime: %lf \n",p->sysTime);
        #endif
}

void* sysTimer(void *arg) {
    // arg -> to temporary, just a curiosity
    smartline* sysTemp = (smartline*)arg;
    double incrementum = sysTemp -> timerIncrementum;
    double divider = sysTemp -> timerDivider;
    // timer
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = (long)((incrementum * divider) * 1e9);
    while (true) {
        // ts.tv_nsec itt, ha nincs recall
        nanosleep(&ts, NULL);
        sysTimerHandler();
    }
}

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
    if (timer_settime(timerid, 0, &this->its, NULL) == -1) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with timer_settime --> smartLineMake");
        #endif
        return false;
    }
    #elif _WIN32 || _WIN64
    if (pthread_create(&this->sysTimerThread, NULL, sysTimer, (void *)this) != 0) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with sysTimer-thread. (pthread_create --> smartLineMake)");
        #endif
        message("smartLineMake_DEFAULT_ERROR");
        return false;
    }
    #endif

    return true;
}

void smartLineInit(smartline *s) {
    psmartline = s;
}

bool smartLineMake(smartline *this) {

    // timer
    this->timerDivider = 1.0;
    sysTimerStart(this);


    pthread_exit(NULL);
    return true;
}
