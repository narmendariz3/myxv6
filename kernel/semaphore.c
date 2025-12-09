#include "types.h"
#include "riscv.h"
#include "param.h"
#include "defs.h"
#include "spinlock.h"
#include "semaphore.h"

struct semtab semtable;

// Initialize semaphore table at kernel boot
void seminit(void) {
    initlock(&semtable.lock, "semtable");
    for (int i = 0; i < NSEM; i++) {
        initlock(&semtable.sem[i].lock, "semlock");
        semtable.sem[i].valid = 0;   // initially invalid
        semtable.sem[i].count = 0;
    }
    printf("[DEBUG] seminit: initialized %d semaphores\n", NSEM);
}

// Allocate a semaphore; return index or -1 if none free
int semalloc(void) {
    acquire(&semtable.lock);
    for (int i = 0; i < NSEM; i++) {
        if (semtable.sem[i].valid == 0) {
            // Do NOT set valid yet; leave that for sys_sem_init
            release(&semtable.lock);
            printf("[DEBUG] semalloc: allocated semaphore %d\n", i);
            return i;
        }
    }
    release(&semtable.lock);
    return -1;
}

// Deallocate semaphore by index
void semdealloc(int index) {
    if (index < 0 || index >= NSEM)
        return;

    acquire(&semtable.sem[index].lock);
    semtable.sem[index].valid = 0;
    semtable.sem[index].count = 0;
    wakeup(&semtable.sem[index]);
    release(&semtable.sem[index].lock);

    printf("[DEBUG] semdealloc: deallocated semaphore %d\n", index);
}
