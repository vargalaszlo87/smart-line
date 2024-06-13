#include <assert.h>
#include <stdint.h>

#include "entity.h"
#include "smartline.h"

extern setup* psetup;

bool entityMake(entity *this) {
    #ifdef NDEBUG
        assert(this->capacity.working < UINT32_MAX);
        assert(this->capacity.inWaiting < UINT32_MAX);
        assert(this->capacity.outWaiting < UINT32_MAX);
    #endif
    this->capacity.item = (uint32_t*)calloc(this->capacity.inWaiting + this->capacity.outWaiting + this->capacity.working,sizeof(uint32_t));
    this->capacity.itemID = (uint32_t*)calloc(this->capacity.inWaiting + this->capacity.outWaiting + this->capacity.working,sizeof(uint32_t));
    if (!this -> capacity.item || !this -> capacity.itemID) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with memory allocation. (entityMake)");
        #endif
        return false;
    }
    this->interface.inputBlocked = false;
    return true;
}

bool entityTakt(entity *this) {
    return (this->time.job - this->time.counter == psetup->incrementum) ? true : false;
}


bool entityRunning(entity *this) {    while (true) {
        // command
        if (entityGetStatus(this) != RUN)
            break;
        // check
            // ha üres --> continue
            if (entityIsEmpty(this))
                continue;
            // blokkolt a bemenete a következõ elemnek?
            //if ()



    }
    return true;
}

bool entityIsEmpty(entity *this) {
    uint64_t length = this->capacity.inWaiting + this->capacity.outWaiting + this->capacity.working;
    uint32_t i = 0;
    bool isEmpty = false;
    while (this->capacity.item[i] < length) {
        if (this->capacity.item[i] == 0) {
            return true;
        }
        i++;
    }
}

bool entityRun(entity *this) {
    this->status = RUN;
}

bool entityStop(entity *this) {
    this->status = STOP;
}

bool entitySetStatus(entity *this, int8_t status) {
    this->status = status;
}

int8_t entityGetStatus(entity *this) {
    return this->status;
}

