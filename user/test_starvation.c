#include "types.h"
#include "stat.h"
#include "user.h"

#define HIGH_PRIORITY 10

int main(int argc, const char *argv[])
{
  // Explain the test
  printf(1, "%s test program\n", argv[0]);
  printf(1, "Pass: the low-priority process never outputs to console\n");
  printf(1, "Fail: the low-priority process gets a chance to run and outputs to console\n\n");


  // Create 5 high-priority CPU-bound children
  for (int i = 0; i < 5; ++i)
  {
    if (fork() == 0)
    {
      // Set each child to the same high priority
      nice(getpid(), HIGH_PRIORITY);
      // Burn CPU
      while (1) {}
    }
  }

  // Fork one low-priority process
  if (fork() == 0)
  {
    // Set this child to a low priority
    nice(getpid(), 1);
    while (1)
    {
      // If we see this message, then the low-priority process is getting CPU time,
      // which means the test failed.
      printf(1, "Low priority process running!\n");
      sleep(100);
    }
  }

  wait(); // parent waits forever
  exit();
}
