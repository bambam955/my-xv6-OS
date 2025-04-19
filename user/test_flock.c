
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
  // Explain the test
  printf(1, "%s test program\n", argv[0]);
  printf(1, "Pass: output from parent and child processes is NOT interleaved\n");
  printf(1, "Fail: output from processes is interleaved together\n\n");
  

  // Create a new test file that will be used for blocking
  int fd = open(argv[0], O_CREATE | O_RDWR);
  fork();

  // Both parent and child run the following code:

  // Try to get the lock on the file.
  // Either the parent or the child will block here, whichever loses the scheduling race.s
  flock(fd);
  // Whichever one gets here first will do all its output before releasing the lock.
  printf(1, "%d: got file lock!\n", getpid());
  for (int i = 0; i < 10; i++)
  {
    printf(1, "%d: running...\n", getpid());
    // Wait for a little while
    sleep(100);
  }
  printf(1, "%d: unlocking file\n", getpid());
  funlock(fd);

  wait();
  exit();
}
