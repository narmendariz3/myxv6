#define NPROC 64 // for hw3, matches param.h//hw added

#ifndef USER_H
#define USER_H

#include "param.h"   // for NPROC
#include "types.h"
#include "kernel/pstat.h"//hw 3

#define NPROC 64 //hw3


struct stat;
struct rtcdate;
 //hw3 getprocs
//struct pstat {
   // int inuse[NPROC];
   // int pid[NPROC];
   // int ppid[NPROC];
   // int state[NPROC];   // just use int because a;ready declared
   // int size[NPROC];
   // char name[NPROC][16];
  //  int priority[NPROC];
//};
// system calls
int fork(void);
int exit(int) __attribute__((noreturn));
int wait(int*);
int pipe(int*);
int write(int, const void*, int);
int read(int, void*, int);
int close(int);
int kill(int);
int exec(char*, char**);
int open(const char*, int);
int mknod(const char*, short, short);
int unlink(const char*);
int fstat(int fd, struct stat*);
int link(const char*, const char*);
int mkdir(const char*);
int chdir(const char*);
int dup(int);
int getpid(void);
char* sbrk(int);
int sleep(int);
int uptime(void);


int getprocs(struct pstat*); //hw3 getprocs
int setPriority(int pid, int priority); //HOMEOWRK3
int getPriority(int pid);
// ulib.c
int stat(const char*, struct stat*);
char* strcpy(char*, const char*);
void *memmove(void*, const void*, int);
char* strchr(const char*, char c);
int strcmp(const char*, const char*);
void fprintf(int, const char*, ...);
void printf(const char*, ...);
char* gets(char*, int max);
uint strlen(const char*);
void* memset(void*, int, uint);
void* malloc(uint);
void free(void*);
int atoi(const char*);
int memcmp(const void *, const void *, uint);
void *memcpy(void *, const void *, uint);

#endif
