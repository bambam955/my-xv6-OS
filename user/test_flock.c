
#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{
    printf(2, "%s Test passes if output from parent and child processes\n",
           argv[0]);
    printf(2, "is NOT interleaved.\n");
    int fd = open("lock", O_CREATE | O_RDWR);
    fork();
    
    // Both parent and child run the following code
    flock(fd);
    printf(2, "%d got lock\n", getpid());
    for (int i = 0; i < 10; i++)
    {
        printf(2, "%d is running\n", getpid());
        sleep(100);
    }
    printf(2, "%d unlocking\n", getpid());
    funlock(fd);
    wait();
    exit();
}
