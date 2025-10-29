#ifndef PSTAT_H
#define PSTAT_H


#include "param.h"
#include "types.h"

struct pstat {
    int inuse[NPROC];
    int pid[NPROC];
    int ppid[NPROC];
    int state[NPROC];
    uint64 size[NPROC];      // still uint64
    char name[NPROC][16];
    int priority[NPROC];
};

#endif
