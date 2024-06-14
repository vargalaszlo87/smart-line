#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>
#include <pthread.h>

// app
#include "smartline.h"

// debug
#define NDEBUG



int main(int argc, char *argv[]) {

    // smartLINE setup
    smartline smart;
    smart.timeIncrementum = 0.1;

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

    // Step 3: set capacity of machine
    machineA.capacity.inWaiting = 1;
    machineA.capacity.working = 4;
    machineA.capacity.outWaiting = 1;

    // Step 4: set process parameters
    machineA.time.job = 32.4;
    machineA.status = RUN;

    // Start
    entityMake(&machineA);
    // ----------------------------------




    smartLineMake();

    printf ("�res? :%d", entityIsEmpty(&machineA));


    return 0;
}
