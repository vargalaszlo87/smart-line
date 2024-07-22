#include <stdint.h>
#include <stdbool.h>

#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#ifndef M_PI
#define M_PI 3.1415926535
#endif

#include "smartline.h"

extern smartline* psmartline;

typedef struct _entity {
    // ID
        char name[128];
        struct _ID {
            uint16_t own;               // en onallo vagyok
            uint16_t *next;             // tobb iranyba mehet
            uint16_t *previus;          // tobb iranybol johet
        } ID;
    // type
        uint8_t type;
        enum {
            LINE = 0,                   // sorszakasz
            MACHINE = 1,                // egy gep
            MOVING = 2,                 // egy mozgato eszkoz
            STORAGE = 3                 // egy tarolo
        } entity_type;
    // interface
        struct _interface {
            int8_t type;
            enum {
                TYPE_11 = 0,            // egytol jon - egyfele megy
                TYPE_1N = 1,            // egytol jon - tobbfele mehet
                TYPE_N1 = 2,            // tobbfelol jon - egyfele mehet
                TYPE_NN = 3             // tobbfelol jon - tobbfele mehet
            } interface_type;
            int32_t inSize;             // egyszerre ennyi jon be
            int32_t outSize;            // egyszerre ennyi megy ki
            bool inputBlocked;          // nem engedem be
        } interface;
    // params
        struct _capacity {
            uint64_t full;              // a teljes kapacitas
            uint32_t working;           // amennyit egyszerre munkal meg
            uint32_t inWaiting;         // amennyi gepen belul van, de meg munkaciklus elott
            uint32_t outWaiting;        // amennyi gepen belul van, de mar munkaciklus utan
            uint32_t *itemBool;         // gepen beluli a FIFO Queue Boolean
            uint32_t *itemID;           // gepen belul az ID-k
            // mask
            uint32_t mask;
            uint32_t maskPO;
        } capacity;
        struct _time {
            float cycleTime;            // munkaciklus ideje .1s felbontassal
            float cycleTimeCounter;     // munkaciklus szamlaloja .1s felbontassal
            float upTime;               // uzemido szamlaloja
            float downTime;             // uzemszunet szamlaloja
        } time;
    // status
        int8_t status;
        enum {
            // error
            ERROR = -1,                 // rendszerhiba
            // unknow
            UNKNOW = 0,                 // ismeretlen
            // stopped
            STOP = 1,                   // all
            PAUSE = 2,                  // szunet
            // maintenance
            MAINTENANCE = 3,            // karbantartas
            TOOLCHANGE = 4,             // szerszamcsere
            // logistic
            INWAITING = 5,              // bemenetre varakozik
            OUTWAITING = 6,             // kimenetre varakozik
            // other
            THROUGH = 7,                // atengedo uzem
            // normal
            RUN = 99                     // termel
        } status_type;
        uint8_t load;
        enum {
            EMPTY = 0,
            FULL = 1,
            HALF = 2
        } load_type;
} entity;

// signatures

bool entityInit(entity *);
//bool entityMake(entity *, smartline *);
int8_t entityRun(entity *);
int8_t entityStop(entity *);
int8_t entitySetStatus(entity *, int8_t) ;
int8_t entityGetStatus(entity *);
bool entityLoad(entity *, uint8_t);

#endif // ENTITY_H_INCLUDED
