#include <stdint.h>
#include <stdbool.h>

#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#ifndef M_PI
#define M_PI 3.1415926535
#endif

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
        uint32_t working;           // amennyit egyszerre munkal meg
        uint32_t inWaiting;         // amennyi gepen belul van, de meg munkaciklus elott
        uint32_t outWaiting;        // amennyi gepen belul van, de mar munkaciklus utan
        uint32_t *itemBool;         // gepen beluli a FIFO Queue Boolean
        uint32_t *itemID;           // gepen belul az ID-k
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
        // normal
        STOP = 1,                   // all
        RUN = 2,                    // termel
        INWAITING = 3,              // bemenetre varakozik
        OUTWAITING = 4,             // kimenetre varakozik
        // maintenance
        MAINTENANCE = 5,            // karbantartas
        TOOLCHANGE = 6,             // szerszamcsere
        // other
        THROUGH = 7,                // atengedo uzem
    } status_type;
} entity;

// signatures

bool entityInit(entity *);
bool entityMake(entity *);
int8_t entityRun(entity *);
int8_t entityStop(entity *);
int8_t entitySetStatus(entity *, int8_t) ;
int8_t entityGetStatus(entity *);
bool entityIsEmpty(entity *);

#endif // ENTITY_H_INCLUDED
