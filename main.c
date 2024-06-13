#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <math.h>
#include <limits.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <assert.h>

// app
#include "smartline.h"

// debug
#define NDEBUG

// presetup
#define INCREMENTUM 0.1



void smartLineInit(setup *s) {
    psetup = s;
}

int main(int argc, char *argv[]) {

    // smartLINE setup
    setup set;
    set.incrementum = 0.1;

    smartLineInit(&set);


    entity machineA;


    // Setp 1: interface
    machineA.interface.type = TYPE_11;
    machineA.interface.inSize = 1;
    machineA.interface.outSize = 1;
    entityInit(&machineA);

    strncpy(machineA.name, "AF10", 4);

    machineA.ID.own = 10;
//    machineA.ID.previus = {9};
//    machineA.ID.next = {11};
    machineA.type = MACHINE;
    // capcaity
    machineA.capacity.inWaiting = 1;
    machineA.capacity.working = 4;
    machineA.capacity.outWaiting = 1;
    machineA.time.job = 32.4;
    machineA.status = RUN;


    entityMake(&machineA);


    printf ("Üres? :%d", entityIsEmpty(&machineA));


    return 0;
}
