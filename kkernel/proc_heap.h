#ifndef F0A2109A_45CC_4067_A12E_3F066FE3490C
#define F0A2109A_45CC_4067_A12E_3F066FE3490C

#include "param.h"
#include "spinlock.h"
#include "types.h"

// Structure for a node in the max-heap.
// We use a separate structure so that we can swap entries without messing with
// the proc pointers themselves.
struct heap_entry {
  struct proc *p;           // The process structure
  int priority;             // The priority of the process in the entry
  uint32_t timestamp;       // When the entry was added to the heap; used for tiebreakers
};

struct heap {
  struct heap_entry entries[NPROC]; // Array of entries in the heap
  int size;                         // Current size of the heap
  struct spinlock lock;             // Lock to ensure protected access to the heap
};

// Create the heap.
void heap_init(struct heap *h);
// Add a new process to the heap. Specify the timestamp using the tick counter.
void heap_insert(struct heap *h, struct proc *p, uint32_t timestamp);
// Pop the highest priority process off of the heap.
// Tiebreakers are decided by taking the process with the older timestamp.
struct proc* heap_pop(struct heap *h);

#endif /* F0A2109A_45CC_4067_A12E_3F066FE3490C */
