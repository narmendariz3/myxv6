//used for 2nd task in hw3
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int pid = getpid();
    printf("parent PID: %d, setting priority = 17\n", pid);
    setPriority(pid, 17);

    int pid_child = fork();
    if (pid_child == 0) {
        // Child
        printf("Child process, running ps to show priorities:\n");
        char *argv[] = { "ps", 0 };
        exec("ps", argv);
        printf("Exec ps failed\n");  // only prints if exec fails
        exit(1);
    } else if (pid_child > 0) {
        // Parent waits for child
        wait(0);
    } else {
        printf("Fork failed\n");
        exit(1);
    }

    exit(0);
}

