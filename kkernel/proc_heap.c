#include "types.h"
#include "defs.h"
#include "proc.h"
#include "proc_heap.h"

void heap_init(struct heap *h) {
  h->size = 0;
}

// Swap two heap entries. Necessary for percolation.
static void swap(struct heap_entry *a, struct heap_entry *b) {
  struct heap_entry tmp = *a;
  *a = *b;
  *b = tmp;
}

// Compare so that higher priority number comes first.
// If priorities are equal, use timestamp (earlier gets priority for round-robin)
static int compare(struct heap_entry *a, struct heap_entry *b) {
  if (a->priority != b->priority)
    return a->priority > b->priority; // HIGHER = higher priority
  return a->timestamp < b->timestamp;
}

void heap_insert(struct heap *h, struct proc *p, uint32_t timestamp) {
  // Protect heap modifications with this lock.
  acquire(&h->lock);
  
  // Don't add more processes if we have reached the limit.
  if (h->size >= NPROC) {
    release(&h->lock);
    return;
  }

  // Put the new entry at the very end of the array.
  int i = h->size++;
  h->entries[i].p = p;
  h->entries[i].priority = p->priority;
  h->entries[i].timestamp = timestamp;

  // Percolate up to the proper spot in the heap.
  while (i > 0) {
    int parent = (i - 1) / 2;
    if (compare(&h->entries[i], &h->entries[parent])) {
      swap(&h->entries[i], &h->entries[parent]);
      i = parent;
    } else break;
  }

  release(&h->lock);
}

struct proc* heap_pop(struct heap *h) {
  // Protect heap modifications with this lock.
  acquire(&h->lock);

  // If we have no runnable processes, return nothing.
  // This will tell the scheduler to run the idle process, waiting for I/O.
  if (h->size == 0)
  {
    release(&h->lock);
    return 0;
  }

  // Pop the highest priority entry off of the heap.
  struct proc *top = h->entries[0].p;
  // Set the root entry to be the very lowest-priority entry...
  h->entries[0] = h->entries[--h->size];

  // ...Then percolate that entry down to where it should be.
  int i = 0;
  while (1) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int largest = i;

    if (left < h->size && compare(&h->entries[left], &h->entries[largest]))
      largest = left;
    if (right < h->size && compare(&h->entries[right], &h->entries[largest]))
      largest = right;

    if (largest != i) {
      swap(&h->entries[i], &h->entries[largest]);
      i = largest;
    } else break;
  }
  release(&h->lock);

  return top;
}
