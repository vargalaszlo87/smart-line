#include <assert.h>
#include <stdint.h>
#include <math.h>
#include <string.h>

#include "entity.h"

// others

void* pointerFromID(smartline* s, uint16_t entityID) {
    uint16_t indexID = 0;
    for (indexID = 0 ; indexID < s->entitySize ; indexID++) {
        if (s->entityID[indexID] == entityID) {
            break;
        }
    }
    return s->entityPointer[indexID];
}

bool shiftRight(entity* this, smartline* s) {
    for (int i = this->capacity.full - 1; i >= 0 ; i--) {
        if (!this -> capacity.itemBool[i])   // if not 1, then continue
            continue;
        if (i == this->capacity.full - 1) {  // the last of the items
            if (this->interface.type == TYPE_11 || this->interface.type == TYPE_N1) {   // one output way
                // checking the first place of the next entity
                // i'm the last entity
                if (s->entityID[s->entitySize-1] == this -> ID.own) {
                    // waiting for the method takeItem()
                    continue;
                }
                // i'm not the last entity
                entity* nextEntityPtr = pointerFromID(s, this->ID.next[0]);
                // if the first stack position of the next entity is not empty, than continue
                if (nextEntityPtr -> capacity.itemBool[0])
                    continue;
                else {
                    nextEntityPtr -> capacity.itemBool[0] = 1;
                    this -> capacity.itemBool[i] = 0;
                    nextEntityPtr -> capacity.itemID[0] = this -> capacity.itemID[i];
                    this -> capacity.itemID[i] = 0;
                }
            }
            else {} // at least two output ways
        }
        else {  // the others of the items
            if (!this -> capacity.itemBool[i + 1]) {
                this -> capacity.itemBool[i + 1] = 1;
                this -> capacity.itemBool[i] = 0;
                this -> capacity.itemID[i + 1] = this -> capacity.itemID[i];
                this -> capacity.itemID[i] = 0;
            }
        }
    }

    return true;
}

// entities

bool entityInit(entity *this) {

/*
    DEV TODO:
        - Az interface típusától függően a ki és bemeneti mennyiségek beállítása
        - Belso szamlalok beallitasa
*/

    // TIME section
    this -> time.cycleTimeCounter = 0;
    this -> time.upTime = 0;
    this -> time.downTime = 0;

    // STATUS section
    this -> status = RUN;

    // INTERFACE section
    this -> ID.own = generateID(8);
    #ifdef NDEBUG
        assert(this->interface.inSize < UINT32_MAX && this->interface.inSize > 0);
        assert(this->interface.outSize < UINT32_MAX && this->interface.outSize > 0);
    #endif
    unsigned short int i, o;
    switch (this->interface.type) {
        case TYPE_11:
            i = 1;
            o = 1;
            break;
        case TYPE_1N:
            i = 1;
            o = this->interface.outSize;
            break;
        case TYPE_N1:
            i = this->interface.inSize;
            o = 1;
            break;
        case TYPE_NN:
            i = this->interface.inSize;
            o = this->interface.outSize;
            break;
        default:
            i = 1;
            o = 1;
            break;
    }
    this->ID.previus = (uint16_t*)calloc(i, sizeof(uint16_t));
    this->ID.next = (uint16_t*)calloc(o, sizeof(uint16_t));
    if (!this -> ID.previus || !this -> ID.next) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with memory allocation. (entityInit)");
        #endif
        message("entityInit_DEFAULT_ERROR");
        return false;
    }
    // default: the entity can receive a work item
    this->interface.inputBlocked = false;

    return true;
}

bool entityMake(entity *this, smartline *s) {

/*
    DEV TODO:
        - OK adja hozzá a smartline-hoz az ID-t.
        - OK Foglalja le a memóriát az gépen belüli FIFO-hoz, inWaiting + working + outWaiting mennyiségek alapján
*/

    // extern : SMARTLINE section
    if (s->entitySize == s->entityCapacity) {
        s->entityCapacity *= 2;
        s->entityID = (uint16_t*)realloc(s->entityID, s->entityCapacity * sizeof(uint16_t));
        s->entityPointer = (void *)realloc(s->entityPointer, s->entityCapacity * sizeof(void));
        if (!s->entityID || !s->entityPointer) {
            #ifdef NDEBUG
                sprintf (stderr, "> Problem with memory re-allication. (Entity Make)");
            #endif // NDEBUG
            message("eneityMake_DEFAULT_ERROR");
            return false;
        }
    }
    s->entityPointer[s->entitySize] = this;
    s->entityID[s->entitySize] = this ->ID.own;
    s->entitySize++;

    // CAPACITY section
    #ifdef NDEBUG
        assert(this->capacity.working < UINT32_MAX);
        assert(this->capacity.inWaiting < UINT32_MAX);
        assert(this->capacity.outWaiting < UINT32_MAX);
    #endif
    if (this->capacity.working < 1) {
        message("entityMake_CAPACITY_NULL");
    }
    this->capacity.full = this->capacity.inWaiting + this->capacity.outWaiting + this->capacity.working;
    this->capacity.itemBool = (uint32_t*)calloc(this->capacity.full,sizeof(uint32_t));
    this->capacity.itemID = (uint32_t*)calloc(this->capacity.full,sizeof(uint32_t));
    if (!this -> capacity.itemBool || !this -> capacity.itemID) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with memory allocation. (entityMake)");
        #endif
        message("entityMake_DEFAULT_ERROR");
        return false;
    }
    // CAPACITY - MASK section
    // mask
    this->capacity.mask = pow(2,this->capacity.full)-1;
    // PO mask
    this->capacity.maskPO = 0;
    for (int i = 0; i < this->capacity.full - this->capacity.inWaiting ; i++) {
        this->capacity.maskPO += pow(2,i);
    }


    return true;
}

bool entityTakt(entity *this) {
    return (this->time.cycleTime - this->time.cycleTimeCounter == psmartline->timerIncrementum) ? true : false;
}

bool entityLoad(entity *this, uint8_t type) {
    uint32_t i = 0, j = 0;
    bool isFull = false;
    while (j < this->capacity.full) {
        if (this->capacity.itemBool[i] == 1)
            i++;
        j++;
    }
    switch (type) {
        case FULL:
            return (this->capacity.full == i) ? true : false;
            break;
        case EMPTY:
            return (i == 0) ? true : false;
            break;
        case HALF:
            return (i > 0 && i < this->capacity.full) ? true : false;
            break;
        default:
            return false;
            break;
    }
}

int8_t entityRun(entity *this) {
    this->status = RUN;
    return this->status;
}

int8_t entityStop(entity *this) {
    this->status = STOP;
    return this->status;
}

int8_t entitySetStatus(entity *this, int8_t status) {
    this->status = status;
    return this->status;
}

int8_t entityGetStatus(entity *this) {
    return this->status;
}

void entityJob(entity *this, smartline *s) {

    // DEV
    entityShowContainers(s);


/*
    DEV TODO:
        - check:
            + ha a status szerint fut, akkor kilep
            + ha ures akkor
            + ha ures akkor interface.inputBlocked = false
            - blokkolt a bemenete a következõ elemnek
            ! maszkok letrehozasa
*/

/*
        // command
        if (entityGetStatus(this) != RUN)
            break;
        // check
        if (entityIsEmpty(this))                    // ha üres --> continue
            continue;
*/



        // stop event's
        // simple
        if (entityGetStatus(this) < 5) {            // the state of entity is ERROR || UNKNOW ||
            return false;                           // STOP || PAUSE || MAINTENANCE || TOOLCHANGE
        }
        // i'm full and the input of the next entity(s) is blocked
        if (entityLoad(this, FULL)) {
            // i'm the last
            if (this -> ID.next[0] == 0)
                return false;
            // finding next route options
            bool nextEntityIsFull = true;
            for (int i = 0; i < (sizeof(this -> ID.next) / sizeof(this -> ID.next[0]) - 1); i++)
            {
                 entity* nextEntityPtr = pointerFromID(s, this->ID.next[i]);
                 if (!nextEntityPtr->interface.inputBlocked) {
                    nextEntityIsFull = false;
                    break;
                 }
                 free(nextEntityPtr);
                 nextEntityPtr = NULL;
            }
            if (nextEntityIsFull == true)
                return false;
        }

        // blocking the input
        // than full
        if (entityLoad(this, FULL) || this->capacity.itemBool[0])  {   // the load of capacity is FULL
            this->interface.inputBlocked = true;
        }
        else {
            this->interface.inputBlocked = false;
        }


        /*else if (entityLoad(this, EMPTY)) {         // the load of capacity is EMPTY
            this->interface.inputBlocked = false;
            return false;
        }*/


        if (this->time.cycleTimeCounter == 0.0) {                         // starting now or has been reset


        }
        else if (this->time.cycleTimeCounter < this->time.cycleTime) {  // production
            // DEV

        }
        else {                                                          // end of takt-time
            shiftRight(this, s);
            this->time.cycleTimeCounter = 0.0;

        }
        // time goes by...
        pthread_mutex_lock(&s->lock);
        this->time.cycleTimeCounter += s->timerIncrementum;
        pthread_mutex_unlock(&s->lock);


   // return true;
}

void entityShowContainers(smartline* this) {


    printf ("\r");
    for (int i = 0 ; i < this->entitySize; i++) {
        entity* temp = this->entityPointer[i];
        printf ("%d ", temp->status);
        for (int j = 0; j < temp->capacity.full ; j++) {
            printf ("%d",temp->capacity.itemBool[j]);
        }
        printf ("\t");
    }
    fflush(stdout);
}



