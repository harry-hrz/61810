#include "kernel/types.h"
#include "user/user.h"

int
main (int argc, char *argv[]) {
  int pc[2];
  int cp[2];
  char buf[10];

  pipe(pc);
  pipe(cp);
  
  if (fork() == 0) {
    int pid = getpid();
    close(cp[0]);
    close(pc[1]);
    read(pc[0], buf, 1);
    printf("%d: received ping\n", pid);
    write(cp[1], "p", 1);
    close(cp[1]);
    close(pc[0]);
    exit(0);
  }
  else {
    int pid = getpid();
    close(cp[1]);
    close(pc[0]);
    write(pc[1], "p", 1);
    read(cp[0], buf, 1);
    printf("%d: received pong\n", pid);
    close(cp[0]);
    close(pc[1]);
    wait(0);
  }
  exit(0);
}
