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
    machineA.time.cycleTime = 8.4;
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
    machineB.ID.next[0] = 12;

    // Step 3: set capacity and time of machine
    machineB.capacity.inWaiting = 2;
    machineB.capacity.working = 3;
    machineB.capacity.outWaiting = 1;

    // Step 4: set process parameters
    machineB.time.cycleTime = 6.4;
    machineB.status = RUN;


    // Make
    entityMake(&machineB, &smart);



    // SmartLINE start
    smartLineStart(&smart);


   // while (1) {}
   pthread_mutex_destroy(&smart.lock);
    return 0;
}
