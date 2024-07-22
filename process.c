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
        first.capacity.itemID[0] = generateID(5);
    }
    return true;
}

// takeItem
// taking the finished item
bool takeItem(smartline *s) {
    if (s->entitySize < 1)          // haven't entity
        return false;
    entity last = *(entity*)s->entityPointer[s->entitySize - 1];
    if (!last.capacity.itemBool[last.capacity.full - 1]) // there is an item at the last position
        return false;
    else {
        last.capacity.itemBool[last.capacity.full - 1] = 0;
        last.capacity.itemID[last.capacity.full - 1] = 0;
    }
    return true;
}

// send
// sending preparation
bool materialHandler(smartline *s) {
    double timerSender = 0.0;
    double timerTaker = 0.0;
    int tempSysTick = s -> sysTick;
    while (true) {
        if (tempSysTick != s -> sysTick) {
            timerSender += s -> timerIncrementum;
            timerTaker += s -> timerIncrementum;
            tempSysTick = s -> sysTick;
        }
        if (timerSender >= s->sendTime) {
            sendItem(s);
            timerSender = 0.0;
        }
        if (timerTaker >= s->takeTime) {
            takeItem(s);
            timerTaker = 0.0;
        }
    }
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

