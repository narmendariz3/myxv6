#include "types.h"
#include "user.h"
#include "pstat.h"
#include "procstate.h"

int main(int argc, char **argv)
{
   // struct pstat uproc[NPROC];
    struct pstat uproc;
    int nprocs;
    int i;
    char *state;
    static char *states[] = {
        [SLEEPING] "sleeping",
        [RUNNABLE] "runnable",
        [RUNNING] "running ",
        [ZOMBIE] "zombie  "};

    nprocs = getprocs(&uproc);
    if (nprocs < 0)
        exit(-1);
//////////////////////////////////task3-hw 3 revision///////////////////////////////

   printf("pid\tstate\t\tsize\tppid\tpriority\tage\tname\n"); // add "age" column
   for (i = 0; i < nprocs; i++)
   {
    if (uproc.inuse[i]) {
        state = states[uproc.state[i]];

        int age = 0;
        if (uproc.state[i] == RUNNABLE) {
            age = uptime() - uproc.readytime[i];  // compute age only if RUNNABLE
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
    //printf("pid\tstate\t\tsize\tppid\tpriority\tname\n");
   // for (i = 0; i < nprocs; i++)
   // {
        //if (uproc.inuse[i]) {
            //state = states[uproc.state[i]];
            //printf("%d\t%s\t%d\t%d\t%d\t\t%s\n",
            //       uproc.pid[i],
          //         state,
        //           uproc.size[i],
      //             uproc.ppid[i],
    //               uproc.priority[i],
  //                 uproc.name[i]);
//        }

   // }

    exit(0);
}
