#ifndef _SPINLOCK_H_
#define _SPINLOCK_H_

// spinlock definition
struct spinlock {
  uint locked;
  char *name;
  struct cpu *cpu;
};

#endif

