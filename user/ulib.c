#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user.h"
#include "kernel/syscall.h"
#include <stdarg.h>

int syscall(int num, ...)
{
    int ret;
    va_list ap;
    va_start(ap, num);

    register uint64 a0 asm("a0");
    register uint64 a1 asm("a1");
    register uint64 a2 asm("a2");
    register uint64 a7 asm("a7") = num;

    a0 = va_arg(ap, uint64);
    a1 = va_arg(ap, uint64);
    a2 = va_arg(ap, uint64);

    asm volatile("ecall"
                 : "+r"(a0)
                 : "r"(a0), "r"(a1), "r"(a2), "r"(a7)
                 : "memory");

    ret = a0;
    va_end(ap);
    return ret;
}

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  char *cdst = (char *) dst;
  int i;
  for(i = 0; i < n; i++){
    cdst[i] = c;
  }
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  if (src > dst) {
    while(n-- > 0)
      *dst++ = *src++;
  } else {
    dst += n;
    src += n;
    while(n-- > 0)
      *--dst = *--src;
  }
  return vdst;
}

int
memcmp(const void *s1, const void *s2, uint n)
{
  const char *p1 = s1, *p2 = s2;
  while (n-- > 0) {
    if (*p1 != *p2) {
      return *p1 - *p2;
    }
    p1++;
    p2++;
  }
  return 0;
}

void *
memcpy(void *dst, const void *src, uint n)
{
  return memmove(dst, src, n);
}



////homework 5
// Semaphore user-space wrappers
int sem_init_user(struct sem_t *s, int value) {
    return syscall(SYS_sem_init, value, (uint64)s);
}

int sem_destroy_user(struct sem_t *s) {
    return syscall(SYS_sem_destroy, (uint64)s);
}

int sem_wait_user(struct sem_t *s) {
    return syscall(SYS_sem_wait, (uint64)s);
}

int sem_post_user(struct sem_t *s) {
    return syscall(SYS_sem_post, (uint64)s);
}


