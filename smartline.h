#ifndef SMARTLINE_H_INCLUDED
#define SMARTLINE_H_INCLUDED

#include "entity.h"

typedef struct _smartline {
    double timeIncrementum;
    double sysTime;
    double Tick;
} smartline;

static smartline* psmartline;

#endif // SMARTLINE_H_INCLUDED
