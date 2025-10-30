#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
  int pid = fork();

  if (pid == 0) {
    // Child
    setPriority(getpid(), 49);  // set child priority high
    while (1) {
      printf("High-prio child ran ( priority = %d , CPU ticks = %d )\n", 49, uptime());
      sleep(10);
    }
  } else {
    // Parent
    setPriority(getpid(), 10);  // parent prio -> low
    while (1) {
      printf("Low-prio parent ran ( priority = %d , CPU ticks = %d )\n", 10, uptime());
      sleep(10);
    }
  }

  exit(0);
}

