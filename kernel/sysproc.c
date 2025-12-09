#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "semaphore.h"

uint64
sys_exit(void)
{
  int n;
  if(argint(0, &n) < 0)
    return -1;
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  if(argaddr(0, &p) < 0)
    return -1;
  return wait(p);
}

uint64
sys_sbrk(void)
{
  int addr;
  int n;
  struct proc *p = myproc();

  if(argint(0, &n) < 0)
    return -1;

  addr = p->sz;
  if (n == 0)
    return addr;

  uint64 new_sz = addr + n;
  if(new_sz < p->sz){
    return (uint64)-1;
  }
  p->sz = new_sz;
  /*old eager allocatoin, we don't call growproc right away for lazy allocatoin*/
  /*if(growproc(n) < 0)
    return -1;*/
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_freepmem(void)
{
  uint64 pages = kfreepages_count();
  return pages * PGSIZE;
}



// homework 5 semaphore adds for task 1 to run
// sys_sem_init(&sem_id, initial_count)
uint64
sys_sem_init(void) {
    int count;
    uint64 uaddr;

    if (argint(0, &count) < 0 || argaddr(1, &uaddr) < 0)
        return -1;

    int idx = semalloc();
    if (idx < 0)
        return -1;

    // Initialize safely
    acquire(&semtable.sem[idx].lock);
    semtable.sem[idx].count = count; // initial count
    semtable.sem[idx].valid = 1;     // mark allocated
    release(&semtable.sem[idx].lock);

    // Copy index back to user space
    if (copyout(myproc()->pagetable, uaddr, (char *)&idx, sizeof(int)) < 0)
        return -1;

    printf("[DEBUG] sys_sem_init: returned idx=%d, count=%d\n", idx, count);
    return 0;
}

// sys_sem_destroy(&sem_id)
uint64
sys_sem_destroy(void) {
    uint64 uaddr;
    int idx;

    if (argaddr(0, &uaddr) < 0)
        return -1;

    if (copyin(myproc()->pagetable, (char *)&idx, uaddr, sizeof(int)) < 0)
        return -1;

    if (idx < 0 || idx >= NSEM)
        return -1;

    semdealloc(idx);
    return 0;
}

// sys_sem_wait(&sem_id)
uint64
sys_sem_wait(void) {
    uint64 uaddr;
    int idx;

    if (argaddr(0, &uaddr) < 0)
        return -1;
    if (copyin(myproc()->pagetable, (char *)&idx, uaddr, sizeof(int)) < 0)
        return -1;
    if (idx < 0 || idx >= NSEM)
        return -1;

    struct semaphore *s = &semtable.sem[idx];
    acquire(&s->lock);
    if (!s->valid) {
        release(&s->lock);
        return -1;
    }

    while (s->count <= 0) {
        sleep(s, &s->lock);
        if (!s->valid) {
            release(&s->lock);
            return -1;
        }
    }

    s->count--;
    release(&s->lock);
    return 0;
}

// sys_sem_post(&sem_id)
uint64
sys_sem_post(void) {
    uint64 uaddr;
    int idx;

    if (argaddr(0, &uaddr) < 0)
        return -1;
    if (copyin(myproc()->pagetable, (char *)&idx, uaddr, sizeof(int)) < 0)
        return -1;
    if (idx < 0 || idx >= NSEM)
        return -1;

    struct semaphore *s = &semtable.sem[idx];
    acquire(&s->lock);
    if (!s->valid) {
        release(&s->lock);
        return -1;
    }

    s->count++;
    wakeup(s);
    release(&s->lock);
    return 0;
}
