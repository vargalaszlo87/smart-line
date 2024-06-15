#include "smartline.h"

void smartLineInit(smartline *s) {
    psmartline = s;
}

void timerHandler(int signal) {
    printf ("Tikk\n");

//    psmartline->startTimer = clock();
}

void *abc(void *vargp) {
    while (1) {
        printf ("Laci\n");
    }

}

bool smartLineMake(smartline *this) {
    //signal(SIGINT, timerHandler);
    pthread_create(&this->sysTimerThread, NULL, abc, (void *)&this->sysTimerThread);



   // pthread_exit(NULL);
    return true;
}
