#ifndef __SYS_V_IPC_H__
#define __SYS_V_IPC_H__
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATHNAME "/tmp/myshm"
#define PROJ_ID 0

int createShm(int size);    // 创建共享内存
int getShm(int size);       // 获取共享内存
int destoryShm(int shmid);  // 销毁共享内存


#endif


// https://zhuanlan.zhihu.com/p/574398364