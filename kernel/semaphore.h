#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include "spinlock.h"
#include "param.h"     // brings in NSEM

struct semaphore {
    struct spinlock lock;
    int valid;
    int count;
};

struct semtab {
    struct spinlock lock;
    struct semaphore sem[NSEM];
};

extern struct semtab semtable;

#endif
