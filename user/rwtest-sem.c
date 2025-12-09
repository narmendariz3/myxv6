#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NULL 0
#define READER_ITERS 50
#define WRITER_ITERS 100

typedef struct {
  int value;               // shared "database" value
  int readercount;         // number of active readers
  struct sem_t mutex;      // protects readercount
  struct sem_t wrt;        // writers' lock (and readers' first/last lock)
} rw_t;

rw_t *rw;

void reader(void) {
  for (int i = 0; i < READER_ITERS; i++) {
    sem_wait_user(&rw->mutex);
    rw->readercount++;
    if (rw->readercount == 1)
      sem_wait_user(&rw->wrt); // first reader blocks writers
    sem_post_user(&rw->mutex);

    // Critical section: reading the shared value
    int val = rw->value;
    (void) val; // suppress unused variable warning

    sem_wait_user(&rw->mutex);
    rw->readercount--;
    if (rw->readercount == 0)
      sem_post_user(&rw->wrt); // last reader allows writers
    sem_post_user(&rw->mutex);
  }
  exit(0);
}

void writer(void) {
  for (int i = 0; i < WRITER_ITERS; i++) {
    sem_wait_user(&rw->wrt); // exclusive access
    rw->value += 1;
    sem_post_user(&rw->wrt);
  }
  exit(0);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    printf("usage: %s <nreaders> <nwriters>\n", argv[0]);
    exit(0);
  }

  int nreaders = atoi(argv[1]);
  int nwriters = atoi(argv[2]);
  int i;

  rw = (rw_t *) mmap(NULL, sizeof(rw_t),
                     PROT_READ | PROT_WRITE,
                     MAP_ANONYMOUS | MAP_SHARED, -1, 0);
  if (rw == (void *)-1 || rw == NULL) {
    printf("rw-sem: mmap failed\n");
    exit(1);
  }

  // initialize shared state
  rw->value = 0;
  rw->readercount = 0;
  sem_init_user(&rw->mutex, 1);
  sem_init_user(&rw->wrt, 1);

  // fork readers
  for (i = 0; i < nreaders; i++) {
    if (!fork()) {
      reader();
    }
  }

  // fork writers
  for (i = 0; i < nwriters; i++) {
    if (!fork()) {
      writer();
    }
  }

  // wait for all children
  for (i = 0; i < nreaders + nwriters; i++)
    wait(0);

  // check final value
  int final = rw->value;
  int expected = nwriters * WRITER_ITERS;
  printf("rw-sem: final value = %d, expected = %d\n", final, expected);

  // cleanup
  sem_destroy_user(&rw->mutex);
  sem_destroy_user(&rw->wrt);
  munmap(rw, sizeof(rw_t));

  exit(0);
}
