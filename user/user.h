#ifndef USER_H
#define USER_H

#include "kernel/syscall.h"  

struct stat;
struct rtcdate;

// hw 5 addition to run 
int syscall(int num, ...);

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
uint64 freepmem(void);

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
void *mmap(void *addr, uint length, int prot, int flags, int fd, int offset); // HOMEWORK 5, mmap and munmap
int   munmap(void *addr, uint length);  // HOMEWORK 5, mmap and munmap

// hw 5 - semaphore structures and prototypes
struct sem_t {
    int idx;    // stores the kernel semaphore index
};

// user-space wrapper prototypes (call these in your user code)
int sem_init_user(struct sem_t *s, int value);
int sem_destroy_user(struct sem_t *s);
int sem_wait_user(struct sem_t *s);
int sem_post_user(struct sem_t *s);

// syscall prototypes (internal, used by usys.S)
uint64 sys_sem_init(int count, uint64 uaddr);
uint64 sys_sem_destroy(uint64 uaddr);
uint64 sys_sem_wait(uint64 uaddr);
uint64 sys_sem_post(uint64 uaddr);

#endif
