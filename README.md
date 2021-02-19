# Lab01 笔记

具体题目可以参考：https://pdos.csail.mit.edu/6.828/2020/labs/util.html

### Sleep.c

``` C
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
```

这道题没啥好说的

### pingpong.c

这道lab我用了一个比较绕的方法，用了两pipe（其实大可不必）。Parent和childern在传输数据的时候分别使用两个pipe，就不会出现夫进程把数据写入之后再读到自己的程序的情况。如果只用一个pipe的话可以参考下列代码:

``` C
int main(int argc, char *argv[]) {
  int p[2]; 
  int pid; 
  char content;
  if(argc > 1){
    fprintf(2, "usage: pingpong\n");
    exit(1);
  }
  pipe(p);
  if (fork() == 0) {
      // child
      pid = getpid();
      // waiting 1 byte from parent
      read(p[0],&content,1);
      close(p[0]);
      printf("%d: received ping\n",pid);
      // send 1 byte to parent 
      write(p[1],"0",1);
      close(p[1]);
  } else {
      // parent
      pid = getpid();
      // send 1 byte to child
      write(p[1],"0",1);
      close(p[1]);
      // IMPORTANT
      // should wait for child here
      // or the parent will read the data write by itself
      wait(0);
      // waiting 1 byte from child
      read(p[0],&content,1);
      close(p[0]);
      printf("%d: received pong\n",pid);
  }
  exit(0);
}
```
来自：https://zhuanlan.zhihu.com/p/346495822

这个代码的流程为： 创建pipe --> 夫进程向pipe写入数据 --> 夫进程等待子进程传回数据 --> 子进程从pipe读取数据 --> 子进程向pipe写入数据后exit --> 子进程返回后夫进程开始读取pipe中的数据

这个里面重要的一步是夫进程等待子进程返回的过程，如果夫进程直接读取pipe中的数据，那么他有可能读取到自己刚才传入的数据

### Primes

这个程序本质是递归的调用子进程。每个子程序接受一组数，第一个数必为prime，接下来的所有数如果不能被prime整除，那么就传到下一个子进程里。如果能被整除，说明不是prime，直接drop。Base case是如果没有从父进程接收到数字，那么就直接返回。

伪代码如下:
```
p = get a number from left neighbor
print p
loop:
    n = get a number from left neighbor
    if (p does not divide n)
        send n to right neighbor
```

这里要注意在递归函数中由于子进程需要通过pipe向父进程通信，所以需要将pipe的file descriptor作为参数传入，这个在递归函数中使用pipe的idiom有必要注意一下。

### find

这个程序主要是`user/ls.c`的修改, 需要注意一下读取目录的方式：
``` C
read(fd, &de, sizeof(de)) == sizeof(de))
```

### xargs

本身对于argv的处理并没有什么难度，扫描文件的过程需要注意一下:
``` C
while ((len = read(0, &buff[count], 1)) != 0 &&
    buff[count] != '\n') {
    count++;  
}

// Reach EOF
if (count == 0) {
  it(0);
}
```
这里文件reach EOF的判定条件并不是当`len = 0`, 而是这个计算buff长度的count等于0。因为即使是len等于0, 也还是有可能出现还有最后一行命令没有执行的情况出现

同时需要注意的还有这个操作:
``` C
buff[count] = 0;
arg_combined[i - 1] = buff;
```
这里直接把读取的一行的最后一个字节改成0是因为系统是靠argument中的`\0`来判断string长度的。

