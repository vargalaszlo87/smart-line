#include <assert.h>
#include <stdint.h>

#include "entity.h"

/*

DEV

// Új elem hozzáadása a dinamikus tömbhöz
void add_element(DynamicArray *arr, int element) {
    // Ha a tömb megtelt, újraméretezzük
    if (arr->size == arr->capacity) {
        arr->capacity *= 2; // Kétszeresére növeljük a kapacitást
        arr->array = (int *)realloc(arr->array, arr->capacity * sizeof(int));
        if (arr->array == NULL) {
            perror("Failed to realloc memory");
            exit(EXIT_FAILURE);
        }
    }
    // Új elem hozzáadása a tömbhöz
    arr->array[arr->size++] = element;
}

DEV 2

#include <stdio.h>

void megnovel(int* ptr) {
   (*ptr)++;
}

int main() {
    int var1 = 42;
    float var2 = 3.14;
    char var3 = 'a';

    // Tömb pointerek tárolására
    void* pointers[3];

    // Pointerek hozzáadása a tömbhöz
    pointers[0] = &var1;
    pointers[1] = &var2;
    pointers[2] = &var3;

    // Kiírjuk a tömbben tárolt címeket
    for (int i = 0; i < 3; i++) {
        printf("Pointer %d address: %p\n", i, pointers[i]);
    }

    megnovel (pointers[0]);

    // Kiírjuk a tényleges értékeket a pointereken keresztül
    printf("Value of var1: %d\n", *(int*)pointers[0]);
    printf("Value of var2: %.2f\n", *(float*)pointers[1]);
    printf("Value of var3: %c\n", *(char*)pointers[2]);

    return 0;
}

*/

int generateID(int length) {
    int out = 0;
    for (int i = 0; i < length; i++) {
        out += (1 + rand() % 9) * pow(10, i);
    }
    return out;
}

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

    void* teszt[3];

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
    return (this->time.cycleTime - this->time.cycleTimeCounter == psmartline->timerIncrementum) ? true : false;
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

    return true;
}

