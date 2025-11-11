// kalloc.h — Physical memory allocator interface

void  kinit(void);
void* kalloc(void);
void  kfree(void *pa);
uint64 freepmem(void);  // HOMEWORK 4
