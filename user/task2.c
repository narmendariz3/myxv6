//used for 2nd task in hw3

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pidA, pidB, pidC;

  pidA = fork();
  if (pidA == 0) {
    setPriority(getpid(), 40);
    for (int i = 0; i < 5; i++) {
      printf("Child A ran ( priority = %d , CPU ticks = %d)\n", 40, uptime());
      sleep(1);
    }
    exit(0);
  }

  pidB = fork();
  if (pidB == 0) {
    setPriority(getpid(), 39);
    for (int i = 0; i < 5; i++) {
      printf("Child B ran ( priority = %d , CPU ticks = %d)\n", 39, uptime());
      sleep(1);
    }
    exit(0);
  }

  sleep(1); // let A and B start before creating C

  pidC = fork();
  if (pidC == 0) {
    setPriority(getpid(), 40);
    for (int i = 0; i < 5; i++) {
      printf("Child C ran ( priority = %d , CPU ticks = %d)\n", 40, uptime());
      sleep(1);
    }
    exit(0);
  }

  wait(0);
  wait(0);
  wait(0);

  exit(0);
}

