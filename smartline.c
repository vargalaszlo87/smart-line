#include "smartline.h"

void smartLineInit(smartline *s) {
    psmartline = s;
}

void sysHandler(int signal) {
    if (signal == SIGINT) {
        smartline* p = &psmartline;
        p->sysTick = !p->sysTick;
        p->sysTime += 0.1;
        p->startTimer = clock();
        #ifdef NDEBUG
            printf ("sysTick: %d \n",p->sysTick);
            printf ("sysTime: %lf \n",p->sysTime);
        #endif
    }
    if (signal == SIGTERM) {
        exit(1);
    }
}

void* sysTimer(void* arg) {
    // arg -> to temporary, just a curiosity
    smartline* sysTemp = (smartline*)arg;
    double incrementum = sysTemp -> timerIncrementum;
    // p from psmartline
    smartline* p = &psmartline;
    while (1) {
        double total = (double)(clock() - p->startTimer) / CLOCKS_PER_SEC;
        if (total >= incrementum) {
            raise(SIGINT);
        }
    signal(SIGINT, sysHandler);
    }
}

bool smartLineMake(smartline *this) {
    signal(SIGINT, sysHandler);
    signal(SIGTERM, sysHandler);
    if (pthread_create(&this->sysTimerThread, NULL, sysTimer, (void *)this) != 0) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with sysTimer-thread. (pthread_create --> smartLineMake)");
        #endif
        message("smartLineMake_DEFAULT_ERROR");
        return false;
    }
    pthread_exit(NULL);
    return true;
}
