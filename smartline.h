#ifndef SMARTLINE_H_INCLUDED
#define SMARTLINE_H_INCLUDED

#include "entity.h"

typedef struct _smartline {
    double timeIncrementum;
    double sysTime;
    double sysTick;
    unsigned int16_t entitySize;
    unsigned int16_t* entityID;
} smartline;

static smartline* psmartline;

#endif // SMARTLINE_H_INCLUDED
