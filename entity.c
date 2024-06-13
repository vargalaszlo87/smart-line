#include <assert.h>
#include <stdint.h>

#include "entity.h"
#include "smartline.h"


extern setup* psetup;

bool entityInit(entity *this) {

/*
    DEV TODO:
        - Az interface típusától függően a ki és bemeneti mennyiségek beállítása
*/

    // INTERFACE section
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
            sprintf (stderr, "> Problem with memory allocation. (entitInit)");
        #endif
        message("entityInit_DEFAULT_ERROR");
        return false;
    }
    // default: the entity can receive a work item
    this->interface.inputBlocked = false;

    return true;
}

bool entityMake(entity *this) {

/*
    DEV TODO:
        - Foglalja le a memóriát az gépen belüli FIFO-hoz, inWaiting + working + outWaiting mennyiségek alapján
*/

    // CAPACITY section
    #ifdef NDEBUG
        assert(this->capacity.working < UINT32_MAX);
        assert(this->capacity.inWaiting < UINT32_MAX);
        assert(this->capacity.outWaiting < UINT32_MAX);
    #endif
    if (this->capacity.working < 1) {
        message("entityMake_CAPACITY_NULL");
    }
    this->capacity.itemBool = (uint32_t*)calloc(this->capacity.inWaiting + this->capacity.outWaiting + this->capacity.working,sizeof(uint32_t));
    this->capacity.itemID = (uint32_t*)calloc(this->capacity.inWaiting + this->capacity.outWaiting + this->capacity.working,sizeof(uint32_t));
    if (!this -> capacity.itemBool || !this -> capacity.itemID) {
        #ifdef NDEBUG
            sprintf (stderr, "> Problem with memory allocation. (entityMake)");
        #endif
        message("entityMake_DEFAULT_ERROR");
        return false;
    }

    return true;
}

bool entityTakt(entity *this) {
    return (this->time.job - this->time.counter == psetup->incrementum) ? true : false;
}


bool entityRunning(entity *this) {

    while (true) {

/*
    DEV TODO:
        - check:
            - ha a status szerint fut, akkor kilep
            - ha ures akkor
            - blokkolt a bemenete a következõ elemnek?
*/


        // command
        if (entityGetStatus(this) != RUN)
            break;
        // check
        if (entityIsEmpty(this))                    // ha üres --> continue
            continue;



        // az idő iszkolása
        /* pseudo code

            if belso ora = nulla                    // most indult / vagy nullázva a belső óra

            else if (belso ora < takt ido)          // termeles

            else                                    // lejrat az ido
        */



        */
    }
    return true;
}

bool entityIsEmpty(entity *this) {
    uint64_t length = this->capacity.inWaiting + this->capacity.outWaiting + this->capacity.working;
    uint32_t i = 0;
    bool isEmpty = false;
    while (this->capacity.itemBool[i] < length) {
        if (this->capacity.itemBool[i] == 0) {
            return true;
        }
        i++;
    }
    return isEmpty;
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

bool shiftRight(entity *this) {


}

