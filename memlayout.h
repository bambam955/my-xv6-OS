// Memory layout

#ifndef DA9A6709_87D0_42B6_9361_0FC8F6DC0A58
#define DA9A6709_87D0_42B6_9361_0FC8F6DC0A58

#define EXTMEM 0x100000     // Start of extended memory
#define PHYSTOP 0xE000000   // Top physical memory
#define DEVSPACE 0xFE000000 // Other devices are at high addresses

// Key addresses for address space layout (see kmap in vm.c for layout)
#define KERNBASE 0x80000000          // First kernel virtual address
#define KERNLINK (KERNBASE + EXTMEM) // Address where kernel is linked

#define V2P(a) (((uint32_t)(a)) - KERNBASE)
#define P2V(a) ((void *)(((char *)(a)) + KERNBASE))

#define V2P_WO(x) ((x) - KERNBASE) // same as V2P, but without casts
#define P2V_WO(x) ((x) + KERNBASE) // same as P2V, but without casts

#endif /* DA9A6709_87D0_42B6_9361_0FC8F6DC0A58 */
