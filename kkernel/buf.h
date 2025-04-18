#ifndef BF68B728_9BB0_4332_970D_840C23BD69D6
#define BF68B728_9BB0_4332_970D_840C23BD69D6
#include "fs.h"
#include "sleeplock.h"
#include "types.h"

struct buf {
  int flags;
  uint32_t dev;
  uint32_t blockno;
  struct sleeplock lock;
  uint32_t refcnt;
  struct buf *prev; // LRU cache list
  struct buf *next;
  struct buf *qnext; // disk queue
  uint8_t data[BSIZE];
};
#define B_VALID 0x2 // buffer has been read from disk
#define B_DIRTY 0x4 // buffer needs to be written to disk

#endif /* BF68B728_9BB0_4332_970D_840C23BD69D6 */
