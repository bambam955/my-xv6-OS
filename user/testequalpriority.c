#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

static const int highPriority = 10;

int
main(int argc, char *argv[])
{
    printf(2, "Test passes if parent and child prints are interleaved\n");
    nice(getpid(), highPriority); // Make self high priority
    printf(2, "%d started with priority: %d\n", getpid(), highPriority);

    int childPid = fork();
    if(0 != childPid) {
        // In high priority parent
        // Make child equally high priority
        nice(childPid, 10);
        printf(2, "Child %d started with priority: %d\n", childPid, highPriority);
    }

    // Both parent and child run this code
    for(int i = 0; i < 10; i++) {
        printf(2, "%d running\n", getpid());
        for(int j = 0; j < (1<<20); j++) {

        }
    }

    printf(2, "%d done!\n", getpid());

    wait(); 
    exit();
}
