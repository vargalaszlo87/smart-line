#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

// app
#include "smartline.h"


int main(int argc, char *argv[]) {

// threading
    pthread_t sysTimerThread;
    pthread_t sysMaterialHandlerThread;
    pthread_t sysOperatorThread;
    pthread_mutex_t lock;


// smartLINE Init
    smartline smart;
    smart.timerIncrementum = 0.1;

    smartLineInit(&smart);


// Machine A ------------------------
    // Init
    entity machineA;

    machineA.type = MACHINE;

    // Step 1: defines the interface
    machineA.interface.type = TYPE_11;
    machineA.interface.inSize = 1;
    machineA.interface.outSize = 1;
    entityInit(&machineA);

    // Step 2: set name and IDs
    strncpy(machineA.name, "AF10", 4);
    machineA.ID.own = 10;
    machineA.ID.previus[0] = 9;
    machineA.ID.next[0] = 11;

    // Step 3: set capacity and time of machine
    machineA.capacity.inWaiting = 1;
    machineA.capacity.working = 4;
    machineA.capacity.outWaiting = 2;

    // Step 4: set process parameters
    machineA.time.cycleTime = 3.4;
    machineA.status = RUN;

    // Make
    entityMake(&machineA, &smart);
    // ----------------------------------

// Machine B ------------------------
    // Init
    entity machineB;
    machineB.type = MACHINE;

    // Step 1: defines the interface
    machineB.interface.type = TYPE_11;
    machineB.interface.inSize = 1;
    machineB.interface.outSize = 1;
    entityInit(&machineB);

    // Step 2: set name and IDs
    strncpy(machineB.name, "AF20", 4);
    machineB.ID.own = 11;
    machineB.ID.previus[0] = 10;
    machineB.ID.next[0] = 0;

    // Step 3: set capacity and time of machine
    machineB.capacity.inWaiting = 2;
    machineB.capacity.working = 3;
    machineB.capacity.outWaiting = 1;

    // Step 4: set process parameters
    machineB.time.cycleTime = 2.4;
    machineB.status = RUN;


    // Make
    entityMake(&machineB, &smart);

    // DEV
    printf ("machine A pointer: %p\n", &machineA);
    printf ("machine B pointer: %p\n\n", &machineB);




    // SmartLINE start
//    smartLineStart(&smart);

    // timer init
    smart.timerDivider = 10.0;  // default: 1.0
    smart.sysTime = 0.0;
    smart.sendTime = 7.8;
    smart.takeTime = 6.4;

    // sysTimer start
    if (pthread_create(&sysTimerThread, NULL, sysTimerHandler, &smart) != 0) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with sysTimer-thread. (pthread_create --> smartLineMake)");
        #endif
        message("smartLineMake_DEFAULT_ERROR");
        return false;
    }

    // sysOperator start
 /*   if (pthread_create(&sysMaterialHandlerThread, NULL, sysMaterialHandler, &smart) != 0) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with sysOperator-thread. (pthread_create --> sysMaterialHandler)");
        #endif
        message("SysOperatorStart_DEFAULT_ERROR");
        return false;
    }
*/
    printf ("\ntest\n");

    pthread_join(sysTimerThread, NULL);
//    pthread_join(sysMaterialHandlerThread, NULL);

   // while (1) {}
   pthread_mutex_destroy(&smart.lock);
    return 0;
}
