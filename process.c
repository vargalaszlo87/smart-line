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
// - iterating through all
void process01(smartline* this) {
    for (int i = this->entitySize - 1 ; i >= 0; i--) {
        entityJob(this->entityPointer[i], this);
    }
    // ide kell majd if
    entityShowContainers(this);
}

