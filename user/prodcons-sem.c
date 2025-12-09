#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

#define BSIZE 10
#define MAX 20
#define NULL 0

typedef struct {
    int buf[BSIZE];
    int nextin;
    int nextout;
    int num_produced;
    int num_consumed;
    int total;
    struct sem_t occupied;
    struct sem_t free;
    struct sem_t lock;
} buffer_t;

buffer_t *buffer;

void producer()
{
    while (1) {
        sem_wait_user(&buffer->free);
        sem_wait_user(&buffer->lock);

        if (buffer->num_produced >= MAX) {
            sem_post_user(&buffer->free);
            sem_post_user(&buffer->occupied);
            sem_post_user(&buffer->lock);
            exit(0);
        }

        buffer->num_produced++;
        printf("producer %d producing %d\n", getpid(), buffer->num_produced);
        buffer->buf[buffer->nextin++] = buffer->num_produced;
        buffer->nextin %= BSIZE;

        sem_post_user(&buffer->occupied);
        sem_post_user(&buffer->lock);
    }
}

void consumer()
{
    while (1) {
        sem_wait_user(&buffer->occupied);
        sem_wait_user(&buffer->lock);

        if (buffer->num_consumed >= MAX) {
            sem_post_user(&buffer->occupied);
            sem_post_user(&buffer->free);
            sem_post_user(&buffer->lock);
            exit(0);
        }

        printf("consumer %d consuming %d\n", getpid(), buffer->buf[buffer->nextout]);
        buffer->total += buffer->buf[buffer->nextout++];
        buffer->nextout %= BSIZE;
        buffer->num_consumed++;

        sem_post_user(&buffer->free);
        sem_post_user(&buffer->lock);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        printf("usage: %s <nproducers> <nconsumers>\n", argv[0]);
        exit(0);
    }

    int nproducers = atoi(argv[1]);
    int nconsumers = atoi(argv[2]);

    buffer = (buffer_t *) mmap(NULL, sizeof(buffer_t), 
                               PROT_READ | PROT_WRITE,
                               MAP_ANONYMOUS | MAP_SHARED, -1, 0);

    buffer->nextin = 0;
    buffer->nextout = 0;
    buffer->num_produced = 0;
    buffer->num_consumed = 0;
    buffer->total = 0;

    sem_init_user(&buffer->occupied, 0);
    sem_init_user(&buffer->free, BSIZE);
    sem_init_user(&buffer->lock, 1);

    for (int i = 0; i < BSIZE; i++)
        buffer->buf[i] = 0;

    for (int i = 0; i < nconsumers; i++)
        if (!fork()) consumer();

    for (int i = 0; i < nproducers; i++)
        if (!fork()) producer();

    for (int i = 0; i < nconsumers; i++)
        wait(0);

    for (int i = 0; i < nproducers; i++)
        wait(0);

    printf("total = %d\n", buffer->total);

    sem_destroy_user(&buffer->occupied);
    sem_destroy_user(&buffer->free);
    sem_destroy_user(&buffer->lock);

    munmap(buffer, sizeof(buffer_t));
    exit(0);
}
