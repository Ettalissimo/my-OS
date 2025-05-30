#ifndef __SYS_H__
#define __SYS_H__

#include <unistd.h>  

void init_syscall(void);


int sys_example(void);
int sys_shutdown(int n);
int sys_write(const char *s, int len);

#endif
