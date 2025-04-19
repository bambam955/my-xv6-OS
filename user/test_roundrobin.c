#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, const char *argv[])
{
  // Explain the test
  printf(1, "%s test program\n", argv[0]);
  printf(1, "Pass: processes run in order over and over again, with no process getting more CPU schedules than another\n");
  printf(1, "Fail: one process runs more than once in a row, which would signify a broken round-robin scheduler\n\n");

  
  // Create three child processes, all with the same priority
  for (int i = 0; i < 3; ++i) {
    if (fork() == 0) {
      nice(getpid(), 5);
      while (1) {
        // Output each time a process gets CPU time
        printf(1, "Process %d running!\n", i);
        // Wait for a little while 
        sleep(100);
      }
    }
  }

  wait(); // parent waits forever
  exit();
}
