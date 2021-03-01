# Lab02 notes

由于本人关于操作系统system call方面的了解几乎为0，所以这个实验大量参考了https://zhuanlan.zhihu.com/p/274369780 和 https://www.zhihu.com/people/bodylangeuages/posts 的代码，非常感谢他们对于代码详细的解释

### Trace
这个实验主要让我对用户层和内核层如何交互有了一个大致的感觉。具体的代码就略过，直接进入trace的流程分析

用户调用`trace`函数(在`user.h`中declare) --> trace 函数被usys.pl转化成汇编指令，跳转到内核层 --> 内核层的`syscall()`从trapframe中读取刚刚被汇编指令存取的system call指令(在`syscall.c`中）--> syscall根据刚刚读到的指令执行在`sysproc.c`中实现的`sys_trace()`

具体不同文件的作用:
`trace.c`: 用户层实现的trace程序，arguments分别是需要追踪的system call和
`usys.pl`: 将system call转化成汇编指令传递到内核层
`syscall.c`: System call 函数本身的实现 
`sysproc.c`: 各种system call的具体实现
`proc.c`: Process structure和一些其他实现



