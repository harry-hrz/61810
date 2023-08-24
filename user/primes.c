#include "kernel/types.h"
#include "user/user.h"

int isPrime (int x) {
  for (int i = 2; i < x; i++) {
    if (x % i == 0) {
      return 0;
    }
  }
  return 1;
}

int main (int argc, char *argv[]) {
  int p[2];
  int c[2];

  pipe(p);
  pipe(c);

  if (fork() == 0) {
    close(p[1]);
    close(c[0]);
    int num;
    while (read(p[0], &num, sizeof(int))) {
      if (isPrime(num)) {
        if (fork() == 0) {
          write(c[1], &num, sizeof(int));
          close(c[1]);
          exit(0);
        }
        wait(0);
      }
      if (num == 35) break;
    }
    close(p[0]);
    close(c[1]);
    exit(0);
  }
  else {
    close(p[0]);
    close(c[1]);
    int prime;
    for (int i = 2; i <= 35; i++) {
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
    while (read(c[0], &prime, sizeof(int))) {
      printf("prime %d\n", prime);
    }
    close(c[0]);
    wait(0);
  }
  exit(0);
}
