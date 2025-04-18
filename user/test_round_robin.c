#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_PROCS 3

int
main(void)
{
  for (int i = 0; i < NUM_PROCS; ++i) {
    if (fork() == 0) {
      nice(getpid(), 5); // Equal priority for all
      while (1) {
        printf(1, "Process %d running!\n", i);
        sleep(100);
      }
    }
  }

  wait(); // parent waits forever
  exit();
}
