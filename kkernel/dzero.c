// "zero" device driver

#include "file.h" // for struct devsw

int dzeroread(struct inode *ip, char *dst, int n)
{
  // Add as many \0 null characters as are asked for to the output buffer.
  for (int i = 0; i < n; ++i)
  {
    dst[i] = '\0';
  }
  return n;
}

void dzeroinit(void)
{
  // No write function required since this is a read-only device
  devsw[DZERO].read = dzeroread;
}
