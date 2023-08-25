#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void
find(char *path, char *fn)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if (!strcmp(fmtname(path), fn)) {
    printf("%s\n", path);
  }

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }


  if (st.type != T_DIR) {
    close(fd);
    return;
  }
  
  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
    printf("find: path too long\n");
    close(fd);
  }
  strcpy(buf, path);
  p = buf+strlen(buf);
  *p++ = '/';
  while(read(fd, &de, sizeof(de)) == sizeof(de)){
    if(de.inum == 0)
      continue;
    memmove(p, de.name, DIRSIZ);
    p[DIRSIZ] = 0;
    if(!strcmp(de.name, ".") || !strcmp(de.name, "..")){
      continue;
    }
    find(buf, fn);
  }
  close(fd);
}

int main (int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(2, "wrong usage, correct one: find PATH FILE_NAME\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}
