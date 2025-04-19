// "null" device driver

#include "file.h" // for struct devsw

int dnullread(struct inode *ip, char *buf, int n)
{
  // Ensure that we don't get errors when trying to read from this device.
  // Returning 0 will effectively act as an immediate EOF.
  return 0;
}

int dnullwrite(struct inode *ip, char *buf, int n)
{
  // We can't return 0 because it will cause kernel panic.
  // So, simply return the number of bytes passed in, as if all of them were "written".
  return n;
}

void dnullinit(void)
{
  devsw[DNULL].read = dnullread;
  devsw[DNULL].write = dnullwrite;
}
