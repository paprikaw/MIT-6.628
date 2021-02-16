#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[]) {

  char *count;
  int num_of_tick;
  if(argc <= 1){
    fprintf(2, "usage: grep pattern [file ...]\n");
    exit(1);
  }

  count = argv[1];
  num_of_tick = atoi(count);
  sleep(num_of_tick);
  exit(0);

}
