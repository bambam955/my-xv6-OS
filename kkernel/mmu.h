// This file contains definitions for the

#ifndef D46FA37D_C064_40BC_98A1_FF66B788A031
#define D46FA37D_C064_40BC_98A1_FF66B788A031

// x86 memory management unit (MMU).

// Eflags register
#define FL_IF 0x00000200 // Interrupt Enable

// Control Register flags
#define CR0_PE 0x00000001 // Protection Enable
#define CR0_WP 0x00010000 // Write Protect
#define CR0_PG 0x80000000 // Paging

#define CR4_PSE 0x00000010 // Page size extension

// various segment selectors.
#define SEG_KCODE 1 // kernel code
#define SEG_KDATA 2 // kernel data+stack
#define SEG_UCODE 3 // user code
#define SEG_UDATA 4 // user data+stack
#define SEG_TSS 5   // this process's task state

// cpu->gdt[NSEGS] holds the above segments.
#define NSEGS 6

#ifndef __ASSEMBLER__
// Segment Descriptor
struct segdesc {
  int lim_15_0 : 16;  // Low bits of segment limit
  int base_15_0 : 16; // Low bits of segment base address
  int base_23_16 : 8; // Middle bits of segment base address
  int type : 4;       // Segment type (see STS_ constants)
  int s : 1;          // 0 = system, 1 = application
  int dpl : 2;        // Descriptor Privilege Level
  int p : 1;          // Present
  int lim_19_16 : 4;  // High bits of segment limit
  int avl : 1;        // Unused (available for software use)
  int rsv1 : 1;       // Reserved
  int db : 1;         // 0 = 16-bit segment, 1 = 32-bit segment
  int g : 1;          // Granularity: limit scaled by 4K when set
  int base_31_24 : 8; // High bits of segment base address
};

// Normal segment
#define SEG(type, base, lim, dpl)                                              \
  (struct segdesc) {                                                           \
    ((lim) >> 12) & 0xffff, (int)(base) & 0xffff,                         \
        ((int)(base) >> 16) & 0xff, type, 1, dpl, 1,                      \
        (int)(lim) >> 28, 0, 0, 1, 1, (int)(base) >> 24              \
  }
#define SEG16(type, base, lim, dpl)                                            \
  (struct segdesc) {                                                           \
    (lim) & 0xffff, (int)(base) & 0xffff,                                 \
        ((int)(base) >> 16) & 0xff, type, 1, dpl, 1,                      \
        (int)(lim) >> 16, 0, 0, 1, 0, (int)(base) >> 24              \
  }
#endif

#define DPL_USER 0x3 // User DPL

// Application segment type bits
#define STA_X 0x8 // Executable segment
#define STA_W 0x2 // Writeable (non-executable segments)
#define STA_R 0x2 // Readable (executable segments)

// System segment type bits
#define STS_T32A 0x9 // Available 32-bit TSS
#define STS_IG32 0xE // 32-bit Interrupt Gate
#define STS_TG32 0xF // 32-bit Trap Gate

// A virtual address 'la' has a three-part structure as follows:
//
// +--------10------+-------10-------+---------12----------+
// | Page Directory |   Page Table   | Offset within Page  |
// |      Index     |      Index     |                     |
// +----------------+----------------+---------------------+
//  \--- PDX(va) --/ \--- PTX(va) --/

// page directory index
#define PDX(va) (((int)(va) >> PDXSHIFT) & 0x3FF)

// page table index
#define PTX(va) (((int)(va) >> PTXSHIFT) & 0x3FF)

// construct virtual address from indexes and offset
#define PGADDR(d, t, o) ((int)((d) << PDXSHIFT | (t) << PTXSHIFT | (o)))

// Page directory and page table constants.
#define NPDENTRIES 1024 // # directory entries per page directory
#define NPTENTRIES 1024 // # PTEs per page table
#define PGSIZE 4096     // bytes mapped by a page

#define PTXSHIFT 12 // offset of PTX in a linear address
#define PDXSHIFT 22 // offset of PDX in a linear address

#define PGROUNDUP(sz) (((sz) + PGSIZE - 1) & ~(PGSIZE - 1))
#define PGROUNDDOWN(a) (((a)) & ~(PGSIZE - 1))

// Page table/directory entry flags.
#define PTE_P 0x001  // Present
#define PTE_W 0x002  // Writeable
#define PTE_U 0x004  // User
#define PTE_PS 0x080 // Page Size

// Address in page table or page directory entry
#define PTE_ADDR(pte) ((int)(pte) & ~0xFFF)
#define PTE_FLAGS(pte) ((int)(pte) & 0xFFF)

#ifndef __ASSEMBLER__
typedef int pte_t;

// Task state segment format
struct taskstate {
  int link:32; // Old ts selector
  int esp0:32; // Stack pointers and segment selectors
  int ss0:16;  //   after an increase in privilege level
  int padding1:16;
  int esp1:32;
  int ss1:16;
  int padding2:16;
  int esp2:32;
  int ss2:16;
  int padding3:16;
  void *cr3;     // Page directory base
  int eip:32; // Saved state from last task switch
  int eflags:32;
  int eax:32; // More saved state (registers)
  int ecx:32;
  int edx:32;
  int ebx:32;
  int esp:32;
  int ebp:32;
  int esi:32;
  int edi:32;
  int es:16; // Even more saved state (segment selectors)
  int padding4:16;
  int cs:16;
  int padding5:16;
  int ss:16;
  int padding6:16;
  int ds:16;
  int padding7:16;
  int fs:16;
  int padding8:16;
  int gs:16;
  int padding9:16;
  int ldt:16;
  int padding10:16;
  int t:16;    // Trap on task switch
  int iomb:16; // I/O map base address
};

// Gate descriptors for interrupts and traps
struct gatedesc {
  int off_15_0 : 16;  // low 16 bits of offset in segment
  int cs : 16;        // code segment selector
  int args : 5;       // # args, 0 for interrupt/trap gates
  int rsv1 : 3;       // reserved(should be zero I guess)
  int type : 4;       // type(STS_{IG32,TG32})
  int s : 1;          // must be 0 (system)
  int dpl : 2;        // descriptor(meaning new) privilege level
  int p : 1;          // Present
  int off_31_16 : 16; // high bits of offset in segment
};

// Set up a normal interrupt/trap gate descriptor.
// - istrap: 1 for a trap (= exception) gate, 0 for an interrupt gate.
//   interrupt gate clears FL_IF, trap gate leaves FL_IF alone
// - sel: Code segment selector for interrupt/trap handler
// - off: Offset in code segment for interrupt/trap handler
// - dpl: Descriptor Privilege Level -
//        the privilege level required for software to invoke
//        this interrupt/trap gate explicitly using an int instruction.
#define SETGATE(gate, istrap, sel, off, d)                                     \
  {                                                                            \
    (gate).off_15_0 = (int)(off) & 0xffff;                                \
    (gate).cs = (sel);                                                         \
    (gate).args = 0;                                                           \
    (gate).rsv1 = 0;                                                           \
    (gate).type = (istrap) ? STS_TG32 : STS_IG32;                              \
    (gate).s = 0;                                                              \
    (gate).dpl = (d);                                                          \
    (gate).p = 1;                                                              \
    (gate).off_31_16 = (int)(off) >> 16;                                  \
  }

#endif

#endif /* D46FA37D_C064_40BC_98A1_FF66B788A031 */
