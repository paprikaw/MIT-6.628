#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


/* pseudocode of each child process:
* p = get a number from left neighbor
* print p
* loop:
*     n = get a number from left neighbor
*     if (p does not divide n)
*         send n to right neighbor
*/
void
child_process (int* input_pipe) {
  
  int output_pipe[2];// Pipe that process write to child
  int prime;

  // Read from parent
  close(input_pipe[1]);
  // Base case: if there is no number received from left neighbor
  if (read(input_pipe[0], &prime, sizeof(int)) == 0) {
    close(input_pipe[0]);
    exit(0);
  }

  // print prime
  printf("prime %d\n", prime);

  // Make up a pipe to transfer data to child
  pipe(output_pipe);
  if (fork() == 0) { //Child process
    child_process(output_pipe);
  } else { //Parent process
    // Close read end of output pipe to child
    close(output_pipe[0]);
    int input_num;
    while(read(input_pipe[0], &input_num, sizeof(int)) != 0){
      if (input_num % prime != 0) {
        write(output_pipe[1], &input_num, sizeof(int));
      }
    }

    // Close used pipe end
    close(output_pipe[1]);
    close(input_pipe[0]);
    wait(0);
    exit(0);
  }
}

int main() {
  int p[2];
  pipe(p);
  
  if (fork() == 0) { // Child
    child_process(p);
  } else { // Parent
    close(p[0]);
    // Input all the number into the child process
    for(int i = 2; i <= 35; i++) {
      write(p[1], &i, sizeof(int));
    }
    close(p[1]);
    wait(0);
  }
  exit(0);
}