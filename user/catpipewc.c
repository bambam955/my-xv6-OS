#ifndef FILEPIPE_C
#define FILEPIPE_C

#include "stat.h"
#include "types.h"
#include "user.h"

// EXAMPLE CODE FROM THE BOOK ON HOW TO PIPE (it actually works, try it! Just uncomment the following line):
// #define TEST_ORIG

#ifdef TEST_ORIG

int main()
{
    int p[2];
    char *argv[2];
    argv[0] = "wc";
    argv[1] = 0;
    pipe(p);
    if (fork() == 0)
    {
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        exec("wc", argv);
    }
    else
    {
        close(p[0]);
        write(p[1], "hello world\n", 12);
        close(p[1]);
        wait();
    }

    exit();
}

#else

char buf[512];

int main(int argc, char **argv)
{
    // We should be reading and word-counting at least one file...
    if (argc < 2)
    {
        exit();
    }

    // Generate file descriptions for the read and write ends of the pipe
    int pipe_fds[2];
    pipe(pipe_fds);

    // fork() will create a child process...
    // This first path is the execution of the child process, and the else is for the parent
    if (fork() == 0)
    {
        // Close the stdin stream and replace it with the read end of the pipe.
        // After this, fd 0 will be the read end of the pipe.
        close(0);
        dup(pipe_fds[0]);

        // Close both of the pipe's file descriptors. Note that this does not close fd 0,
        // only the ORIGINAL descriptors given to the pipe.
        close(pipe_fds[0]);
        close(pipe_fds[1]);

        // Execvp() is the Linux function for calling another process.
        // It replaces the current process with the called process.
        char *args[] = {"wc", 0};
        exec("wc", args);
    }
    else
    {
        // Close the read channel...we won't need it. This is the first leg of the pipe.
        close(pipe_fds[0]);

        // Taken from the "cat" code in xv6, modified for this program...
        int fd, n;
        for (int i = 1; i < argc; ++i)
        {
            if ((fd = open(argv[i], 0)) < 0)
            {
                printf(1, "cat: cannot open %s\n", argv[i]);
                exit();
            }

            while ((n = read(fd, buf, sizeof(buf))) > 0)
            {
                // Instead of writing to stdout, we write to the write fd for the pipe.
                if (write(pipe_fds[1], buf, n) != n)
                {
                    printf(1, "cat: write error\n");
                    exit();
                }
            }
            if (n < 0)
            {
                printf(1, "cat: read error\n");
                exit();
            }
            close(fd);
        }

        // Close the write channel now that we've finished reading all the files.
        close(pipe_fds[1]);
    
        // Wait for the child process to finish up.
        wait();
    }

    exit();
}

#endif

#endif
