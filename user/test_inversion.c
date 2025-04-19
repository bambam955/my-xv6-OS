#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define MID_PRIORITY 5
#define HIGH_PRIORITY 10

int main(int argc, char *argv[])
{
  // Explain the test
  printf(1, "%s test program\n", argv[0]);
  printf(1, "Pass: low-priority gets lock, then mid-priority child completes before high-priority gets lock\n");
  printf(1, "Fail: high-priority process runs before the mid-priority process finishes\n\n");


  // Make self (parent) process highest priority
  nice(getpid(), HIGH_PRIORITY);
  printf(1, "%d: process set to priority %d\n", getpid(), HIGH_PRIORITY);
  // Create a new test file that will be used for blocking
  const int fd = open(argv[0], O_CREATE | O_RDWR);


  const int lowChildPid = fork();
  if (0 != lowChildPid)
  {
    // In high priority parent:

    // Gives low-priority child a change to get the lock.
    sleep(1);

    const int midChildPid = fork();
    if (0 != midChildPid)
    {
      // Still in high-priority parent:

      // Give mid-priority process a middle priority
      nice(midChildPid, MID_PRIORITY);

      // Try to get the lock that the low-priority child already took.
      // This will block until the low-priority process releases.
      flock(fd);
      // We finally got the lock! This means the mid-priority process finished
      // and the low-priority process released its lock.
      printf(1, "%d: high-priority process got lock!\n", getpid());
      funlock(fd);
    }
    else
    {
      // In mid priority child:

      // Stall for a while, and output that we are doing so
      for (int i = 0; i < 10; ++i)
      {
        printf(1, "%d: mid-priority process running...\n", getpid());
        for (int j = 0; j < (1 << 20); ++j) {}
      }
    }
  }
  else
  {
    // In low-priority child:

    // Get the file lock before the high-priority process can
    flock(fd);
    printf(1, "%d: low-priority process got lock!\n", getpid());
    // Stall for a while, and output that we are doing so.
    // During this time the high-priority process will be blocked trying to get the lock.
    for (int i = 0; i < 10; ++i)
    {
      printf(1, "%d: low-priority process running...\n", getpid());
      for (int j = 0; j < (1 << 20); ++j) {}
    }
    funlock(fd);
  }

  printf(1, "%d: process done!\n", getpid());
  wait();
  wait();
  exit();
}
