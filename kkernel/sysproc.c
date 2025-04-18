#include "date.h"
#include "defs.h"
#include "memlayout.h"
#include "mmu.h"
#include "param.h"
#include "proc.h"
#include "types.h"
#include "x86.h"
#include "pinfo.h"

int sys_fork(void) { return fork(); }

int sys_exit(void) {
  exit();
  return 0; // not reached
}

int sys_wait(void) { return wait(); }

int sys_kill(void) {
  int pid;

  if (argint(0, &pid) < 0) {
    return -1;
  }
  return kill(pid);
}

int sys_getpid(void) { return myproc()->pid; }

int sys_sbrk(void) {
  int addr;
  int n;

  if (argint(0, &n) < 0) {
    return -1;
  }
  addr = myproc()->sz;
  if (growproc(n) < 0) {
    return -1;
  }
  return addr;
}

int sys_sleep(void) {
  int n;
  uint32_t ticks0;

  if (argint(0, &n) < 0) {
    return -1;
  }
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n) {
    if (myproc()->killed) {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int sys_uptime(void) {
  uint32_t xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_shutdown(void)
{
  shutdown();
  return 0;
}

int sys_ps()
{
  int pinfosToReturnNumber;
  argint(0, &pinfosToReturnNumber);

  char *arg1_p;
  argptr(1, &arg1_p, 1);
  struct pinfo *pinfo_p = (struct pinfo *)arg1_p;

  return cps(pinfosToReturnNumber, pinfo_p);
}

int sys_nice(void)
{
  int pidNum;
  argint(0, &pidNum);

  int priorityNum;
  argint(1, &priorityNum);

  return cnice(pidNum, priorityNum);
}

