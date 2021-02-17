#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
  int to_child_pipe[2]; // Pipe that deliever data to the child 
  int from_child_pipe[2]; // Pipe that receive the data from child
  pipe(to_child_pipe);
  pipe(from_child_pipe);

  int pid = fork();
  if (pid == 0) {

    char receiver[1];
    int child_pid;
    
    close(to_child_pipe[1]);
    read(to_child_pipe[0], receiver, 1);
    close(to_child_pipe[0]);
    // Get child's process id
    child_pid = getpid();

    // chile print message
    printf("%d: received ping\n", child_pid);

    // child resend message to the parent
    close(from_child_pipe[0]);
    write(from_child_pipe[1], receiver, 1);
    close(from_child_pipe[1]);
    exit(1);
  } else {
    char receiver[1];
    int child_pid;
    // Parent send byte to the child
    close(to_child_pipe[0]);
    write(to_child_pipe[1], "b", 1);
    close(to_child_pipe[1]);

    close(from_child_pipe[1]);
    read(from_child_pipe[0], receiver, 1);
    child_pid = getpid();
    printf("%d: received pong\n", child_pid);
    close(from_child_pipe[0]);

    exit(1);
  }

  


}
