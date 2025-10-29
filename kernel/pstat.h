#ifndef PSTAT_H
#define PSTAT_H //to use pstat definition in user.h
#include "param.h"
#include "types.h"

//struct pstat {
  //int inuse[NPROC];     // Whether this slot is in use (1 or 0)
  //int pid[NPROC];       // Process ID
  //int ppid[NPROC];      // Parent process ID
  //int state[NPROC];     // Process state (as an int)
  //uint64 size[NPROC];   // Size of process memory
  //char name[NPROC][16]; // Process name
  //int priority[NPROC];  // Process priority
//};
struct pstat {
  int inuse[NPROC];       // whether this slot is active
  int pid[NPROC];         // process ID
  int ppid[NPROC];        // parent process ID
  int state[NPROC];       // process state (integer)
  uint64 size[NPROC];     // memory size in bytes
  char name[NPROC][16];   // process name
  int priority[NPROC];    // process priority
};
#endif // PSTAT_H
