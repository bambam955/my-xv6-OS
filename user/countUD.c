#include "stat.h"
#include "types.h"
#include "user.h"

static char buf[512];

int main(int argc, char* argv[])
{
    // Output how many files were passed in to be read.
    printf(1, "Processing %d files...\n", argc - 1);

    // Initialize character counters.
    int countU = 0, countD = 0, countOthers = 0;

    // Loop through the filenames passed in as arguments.
    // We start with index 1 because the first "argument" is actually the name of the program. i.e. ./countUD.
    for (int i = 1; i < argc; i++)
    {
        // Open the next file in read-only mode since we don't need to modify it.
        int fd = open(argv[i], 0);
        // Handle the possibility that the file doesn't exist. We don't want to read nonexistent files.
        if (fd == -1)
        {
            printf(1, "No file found with name \'%s\', skipping to next file.\n", argv[i]);
            continue; // Skip to the next given file.
        }

        printf(1, "Reading file \'%s\' ...\n", argv[i]);

        // Initialize the buffer that each byte of each file will be written to.
        // We use an array of chars so that we can avoid calling read() for every single byte.
        // This of course increases the space complexity but dramatically increases the speed of the program.
        int bytesRead = 0;
        while (1)
        {
            // Attempt to read BUF_SIZE bytes (each equal to one ASCII character) from the opened file.
            // If there are no bytes left to read then exit the loop.
            bytesRead = read(fd, buf, sizeof(buf));
            if (bytesRead < 1)
                break;
            // Determine the value of each read character and increment the relevant counter.
            // Make sure to ignore all possible whitespace characters.
            for (int j = 0; j < (bytesRead > sizeof(buf) ? sizeof(buf) : bytesRead); ++j)
            {
                switch (buf[j])
                {
                case 'U':
                    countU++; break;
                case 'D':
                    countD++; break;
                case '\n':
                case '\r':
                case '\t':
                case '\v':
                case '\f':
                case ' ':
                    break;
                default:
                    countOthers++; break;
                }
            }
        }
        // Close the opened file and output progress.
        close(fd);
        if (i + 1 == argc)
            printf(1, "Finished reading file \'%s\'.\n", argv[i]);
        else
            printf(1, "Finished reading file \'%s\', moving to next file.\n", argv[i]);
    }

    // Output the totals of each counter to the proper text streams.
    // TODO: should I use write(1, ...) and write(2, ...) for printing to the console?
    printf(1, "Found %d U's and %d D's, total %d\n", countU, countD, (countU + countD));
    printf(2, "Found %d other characters\n", countOthers);

    // Return the proper exit code. TODO: I don't think I can specify exit codes in xv6...
    // return ((countU > 0 || countD > 0) ? 0 : 1);
    exit();
}
