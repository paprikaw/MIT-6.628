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
find(char *path, char *des)
{
  char buf[512], *p;
  // Buffer is the space to place path
  // p is a pointer to manipulate buffer
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE: // Base case
    if (strcmp (des, fmtname(path)) == 0)
      printf("%s\n", path);
    return;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || strcmp(de.name,"..") == 0 || strcmp(de.name,".") == 0)
        continue;

      
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;

      // Recursively find directory
      find(buf, des);
    }
    break;
  }
  close(fd);
  return;
}

int
main(int argc, char *argv[])
{
  find(argv[1], argv[2]);
  exit(0);
}

