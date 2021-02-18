#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int
main(int argc, char *argv[]) {

  char *arg_combined[MAXARG];
  char buff[512];

  if (argc <= 1) {
    fprintf(2, "Argument is too less");
    exit(1);
  }

  if (argc > MAXARG - 1) {
    fprintf(2, "Argument is too many");
    exit(1);
  }

  // Fill the argument to the array
  int i;
  for (i = 1; i < argc; i++) {
    arg_combined[i - 1] = argv[i];
  } 

  while(1) {

    int len;
    int count = 0;
    // Scan a line
    while ((len = read(0, &buff[count], 1)) != 0 &&
           buff[count] != '\n') {
      count++;  
    }

    // Reach EOF
    if (count == 0) {
      exit(0);
    }

    // Buffer is overloaded
    if (count > 512) {
      fprintf(2, "Argument is too less");
      exit(0);
    }

    buff[count] = 0;
    arg_combined[i - 1] = buff;
    
    if (fork() == 0) { // Child process
      exec(arg_combined[0], arg_combined);
    } else {
      wait(0);
    }
  }
  exit(0);
}
