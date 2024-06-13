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
        uint16_t own;
        uint16_t next;
        uint16_t previus;
    } ID;
    // type
    uint8_t type;
    enum {
        LINE = 0,
        MACHINE = 1,
        MOVING = 2,
        STORAGE = 3
    };
    // interface
    struct _interface {
        int8_t type;
        enum {
            TYPE_11 = 0,
            TYPE_1N = 1,
            TYPE_N1 = 2,
            TYPE_NN = 3
        };
        int32_t inSize;
        int32_t outSize;
        bool inputBlocked;
    } interface;
    // params
    struct _capacity {
        uint32_t working;
        uint32_t inWaiting;
        uint32_t outWaiting;
        uint32_t *item;
        uint32_t *itemID;
    } capacity;
    struct _time {
        float job;
        float counter;
    } time;
    // status
    int8_t status;
    enum {
        // error
        ERROR = -1,
        // unknow
        UNKNOW = 0,
        // normal
        STOP = 1,
        RUN = 2,
        INWAITING = 3,
        OUTWAITING = 4,
        // maintenance
        MAINTENANCE = 5,
        TOOLCHANGE = 6
    };
} entity;

// signatures

bool entityMake(entity *);
bool entityRun(entity *);
bool entityStop(entity *);
bool entitySetStatus(entity *, int8_t) ;
int8_t entityGetStatus(entity *);
bool entityIsEmpty(entity *);

#endif // ENTITY_H_INCLUDED
