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


    entity e1;

    strncpy(e1.name, "AF10", 4);

    e1.ID.own = 10;
    e1.ID.previus = 9;
    e1.ID.next = 11;
    e1.type = MACHINE;
    e1.capacity.inWaiting = 1;
    e1.capacity.working = 4;
    e1.capacity.outWaiting = 1;
    e1.time.job = 32.4;
    e1.status = RUN;


    entityMake(&e1);
    printf ("Üres? :%d", entityIsEmpty(&e1));


    return 0;
}
