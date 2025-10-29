#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/pstat.h"
#include "user/user.h"

int main(int argc, char **argv)
{
    struct pstat uproc;
    int nprocs;
    int i;
    char *state;
    static char *states[] = {
        [SLEEPING] "sleeping",
        [RUNNABLE] "runnable",
        [RUNNING] "running ",
        [ZOMBIE]  "zombie  "
    };

    // Get process info from kernel
    nprocs = getprocs(&uproc);
    if (nprocs < 0) {
        printf("getprocs failed\n");
        exit(-1);
    }

    // Print header
    printf("pid\tstate\t\tsize\tppid\tpriority\tage\tname\n");

    // Print process table
    for (i = 0; i < nprocs; i++) {
        if (uproc.inuse[i]) {
            state = states[uproc.state[i]];

            int age = 0;
            if (uproc.state[i] == RUNNABLE) {
                age = uptime() - uproc.readytime[i]; // compute age only if RUNNABLE
            }

            printf("%d\t%s\t%d\t%d\t%d\t%d\t\t%s\n",
                   uproc.pid[i],
                   state,
                   uproc.size[i],
                   uproc.ppid[i],
                   uproc.priority[i],
                   age,
                   uproc.name[i]);
        }
    }

    exit(0);
}

