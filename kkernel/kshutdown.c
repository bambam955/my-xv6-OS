/* Kernel part of shutdown/poweroff support */

#include "types.h"
#include "x86.h"
#include "defs.h"

void
shutdown(void)
{
  cprintf("SHUTDOWN\n");
  /*
     This only works in QEMU and assumes QEMU was run 
     with -device isa-debug-exit
   */
  outb(0x501, 0x0);
}
