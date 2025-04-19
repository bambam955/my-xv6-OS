#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define HIGH_PRIORITY 10

int main(int argc, char *argv[])
{
  // Explain the test
  printf(1, "%s test program\n", argv[0]);
  printf(1, "Pass: parent and child outputs are interleaved\n");
  printf(1, "Fail: parent and child outputs are completely separated\n\n");


  // Make self (parent) process highest priority
  nice(getpid(), HIGH_PRIORITY);
  printf(1, "%d: process set to priority %d\n", getpid(), HIGH_PRIORITY);

  int childPid = fork();
  if (0 != childPid)
  {
    // In high-priority parent:

    // Make the child process equally high-priority
    nice(childPid, HIGH_PRIORITY);
    printf(1, "%d: child process set to priority %d\n", childPid, HIGH_PRIORITY);
  }

  // Both parent and child run this code.
  // The outputs should be interleaved because of round-robin scheduling.
  for (int i = 0; i < 10; ++i)
  {
    printf(1, "%d: running...\n", getpid());
    // Wait for a little bit
    sleep(50);
  }

  printf(1, "%d: done!\n", getpid());

  wait();
  exit();
}
