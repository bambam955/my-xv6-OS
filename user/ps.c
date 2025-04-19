#include "types.h"
#include "stat.h"
#include "user.h"
#include "pinfo.h"

// Don't allocate this on the stack because the stack is small and
// this will overflow it
struct pinfo arrayOfPInfo[1000];

int main(int argc, const char *argv[])
{
    int psResult = ps(1000, arrayOfPInfo);
    for (int i = 0; i < psResult; ++i)
    {
        printf(1, "%d %d %s %s\n", arrayOfPInfo[i].pid,
               arrayOfPInfo[i].priority,
               arrayOfPInfo[i].stateStr,
               arrayOfPInfo[i].programNameStr);
    }
    exit();
}
