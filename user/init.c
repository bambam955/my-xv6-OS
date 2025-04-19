// init: The initial user-level program

#include "fcntl.h"
#include "file.h"
#include "stat.h"
#include "types.h"
#include "user.h"

char *argv[] = {"sh", 0};

int main(void) {
  int pid, wpid;

  mkdir("dev");

  if (open("dev/console", O_RDWR) < 0) {
    mknod("dev/console", 1, 1);
    open("dev/console", O_RDWR);
  }
  dup(0); // stdout
  dup(0); // stderr

  // Only 10 device major numbers are allowed by param.h
  int dzerofd = open("dev/zero", O_RDWR);
  if(dzerofd < 0){
    mknod("dev/zero", DZERO, 1); // DZERO is major number, 1 is minor number 
    dzerofd = open("dev/zero", O_RDWR);
  }
  close(dzerofd);
  
  int dnullfd = open("dev/null", O_RDWR);
  if(dnullfd < 0){
    mknod("dev/null", DNULL, 1); // DNULL is major number, 1 is minor number 
    dnullfd = open("dev/null", O_RDWR);
  }
  close(dnullfd);

  int dticksfd = open("dev/ticks", O_RDWR);
  if(dticksfd < 0){
    mknod("dev/ticks", DTICKS, 1); // DTICKS is major number, 1 is minor number 
    dticksfd = open("dev/ticks", O_RDWR);
  }
  close(dticksfd);

  int hellofd = open("dev/hello", O_RDWR);
  if(hellofd < 0){
    mknod("dev/hello", 7, 1); // 7 is major number, 1 is minor number 
    hellofd = open("dev/hello", O_RDWR);
  }
  close(hellofd);

  for (;;) {
    printf(1, "init: starting sh\n");
    pid = fork();
    if (pid < 0) {
      printf(1, "init: fork failed\n");
      exit();
    }
    if (pid == 0) {
      exec("sh", argv);
      printf(1, "init: exec sh failed\n");
      exit();
    }
    while ((wpid = wait()) >= 0 && wpid != pid) {
      printf(1, "zombie!\n");
    }
  }
}
