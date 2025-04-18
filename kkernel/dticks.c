// "ticks" device driver

#include "defs.h" // for ticks and tickslock
#include "file.h" // for struct devsw

// NOTE: I didn't need the full functionality of snprintf, so I just copied the relevant code into here
// so that I could use it to convert integers to strings. See printf.c for more documentation.

static uint32_t
s_getReverseDigits(char *outbuf, uint32_t length, int32_t xx, int32_t base, int32_t sgn)
{
  static char digits[] = "0123456789ABCDEF";
  int32_t i, neg;
  uint32_t x;

  neg = 0;
  if (sgn && xx < 0)
  {
    neg = 1;
    x = -xx;
  }
  else
  {
    x = xx;
  }

  i = 0;
  while (i + 1 < length && x != 0)
  {
    outbuf[i++] = digits[x % base];
    x /= base;
  }

  if (0 == xx && i + 1 < length)
  {
    outbuf[i++] = digits[0];
  }

  if (neg && i < length)
  {
    outbuf[i++] = '-';
  }

  return i;
}

static uint32_t
s_printint(char *outbuf, uint32_t length, int32_t xx, int32_t base, int32_t sgn)
{
  static const uint32_t localBufferLength = 16;
  char localBuffer[localBufferLength];

  uint32_t result =
      s_getReverseDigits(localBuffer, localBufferLength, xx, base, sgn);

  // Reverse the digits to put them in normal print order with most significant
  // digit to the left
  int32_t j = 0;
  int32_t i = result;
  while (--i >= 0 && j < length)
  {
    outbuf[j] = localBuffer[i];
    j++;
  }
  // Ensure the last character is a null (assuming we have room)
  if (result < length)
  {
    outbuf[result] = '\0';
  }
  return result + 1;
}

int dticksread(struct inode *ip, char *dst, int n)
{
  uint32_t ticks0;
  // Get the lock on the internal clock data.
  acquire(&tickslock);
  // Store the value of the tick counter.
  ticks0 = ticks;
  // Release the lock.
  release(&tickslock);
  // Write the integer value to the destination buffer.
  int bytesWritten = s_printint(dst, n, ticks0, 10, 1);
  return bytesWritten;
}

void dticksinit(void)
{
  // No write function required since this is a read-only device
  devsw[DTICKS].read = dticksread;
}
