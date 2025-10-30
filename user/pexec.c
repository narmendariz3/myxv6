#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

// Usage: pexec <priority> <program> [args...]

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    printf("Usage: pexec <priority> <program> [args...]\n");
    exit(1);
  }

  int priority = atoi(argv[1]);

  // Set this process's priority before exec
  setPriority(getpid(), priority);

  exec(argv[2], &argv[2]);

  // Only reaches here if exec fails
  printf("pexec: exec %s failed\n", argv[2]);
  exit(1);
}

