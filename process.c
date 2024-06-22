#include "process.h"

// sendItem
// adding an item to the production line
bool sendItem() {


    return true;
}

// takeItem
// taking the finished item
bool takeItem() {


    return true;
}

// process01
//
void process01(smartline* this) {
    for (int i = 0; i < this->entitySize; i++) {
        entityJob(this->entityPointer[i]);
    }
}
