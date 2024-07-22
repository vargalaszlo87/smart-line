#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include "smartline.h"

extern entityJob();

#include <stddef.h>
#include <stdbool.h>

int generateID(int );
bool sendItem(smartline* );
bool takeItem(smartline* );
bool materialHandler(smartline* );
void process01(smartline* );


#endif // PROCESS_H_INCLUDED
