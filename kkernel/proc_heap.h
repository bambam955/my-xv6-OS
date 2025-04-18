#ifndef F0A2109A_45CC_4067_A12E_3F066FE3490C
#define F0A2109A_45CC_4067_A12E_3F066FE3490C

#include "param.h"
#include "types.h"

struct heap_entry {
  struct proc *p;
  int priority;
  uint32_t timestamp;
};

struct heap {
  struct heap_entry entries[NPROC];
  int size;
};

void heap_init(struct heap *h);
void heap_insert(struct heap *h, struct proc *p, int priority, uint32_t timestamp);
struct proc* heap_pop(struct heap *h);


#endif /* F0A2109A_45CC_4067_A12E_3F066FE3490C */
