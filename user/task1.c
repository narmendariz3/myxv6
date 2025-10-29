#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
    int pid = getpid();
    printf("parent PID: %d, setting priority = 17\n", pid);
    setpriority(pid, 17);

    int pid_child = fork();
    if(pid_child == 0) {
        printf("Child process, running ps to show priorities:\n");
        exec("ps", 0);
        exit(0);
    } else {
        wait(0);
    }
    exit(0);
}
