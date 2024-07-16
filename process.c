#include "process.h"
#include "entity.h"

// generateID
// make an ID for entity or item
int generateID(int length) {
    int out = 0;
    for (int i = 0; i < length; i++) {
        out += (1 + rand() % 9) * pow(10, i);
    }
    return out;
}

// sendItem
// adding an item to the production line
bool sendItem(smartline* s) {
    if (s->entitySize < 1)          // haven't entity
        return false;
    entity first = *(entity*)s->entityPointer[0];
    if (first.capacity.itemBool[0]) // there is an item at the first position
        return false;
    else {
        first.capacity.itemBool[0] = 1;
        first.capacity.itemID = generateID(5);
    }
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
    //entityShowContainers(this);
}

