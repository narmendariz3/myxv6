#ifndef _PSTAT_H_
#define _PSTAT_H_

#include "param.h"
#include "types.h"

// Structure to hold process information for ps-like system calls
struct pstat {
  int pid[NPROC];           // process ID
  int ppid[NPROC];          // parent process ID
  int state[NPROC];         // process state (e.g., RUNNING, SLEEPING)
  int priority[NPROC];      // scheduling priority
  int ticks[NPROC];         // number of ticks this process has run
  int readytime[NPROC];     // time spent waiting to be scheduled
  int size[NPROC];          // memory size of the process
  char name[NPROC][16];     // process name
  int inuse[NPROC];         // whether this slot is used (1 = used)
  int tickets[NPROC];       // lottery tickets (if applicable to your scheduler)
};

#endif // _PSTAT_H_
