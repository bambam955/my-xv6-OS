#include "types.h"
#include "stat.h"
#include "user.h"

#define NUM_HIGH 5
#define NUM_LOW  1

int
main(void)
{
  // Fork high-priority CPU-bound children
  for (int i = 0; i < NUM_HIGH; ++i) {
    if (fork() == 0) {
      nice(getpid(), 10);  // High priority
      while (1)
        ; // Burn CPU
    }
  }

  // Fork one low-priority process
  if (fork() == 0) {
    nice(getpid(), 1);  // Low priority
    while (1) {
      printf(1, "Low priority process running!\n");
      sleep(100);  // Try to show it's getting CPU
    }
  }

  wait(); // parent waits forever
  exit();
}
