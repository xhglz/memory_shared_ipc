#include <unistd.h>
#include "sys_v.h"

// 基于传统SYS V的共享内存
// gcc -o server sys_v_server.c sys_v.c
int main() {
    // 创建共享内存
    int shmid = createShm(4096);
    // 将共享内存进行挂接
    char* addr = (char*)shmat(shmid,NULL,0);
    int i=0;
    while(i++ < 26){
        printf("client: %s\n",addr);
        sleep(1);
    }
    //取挂接
    shmdt(addr);
    sleep(1);
    destoryShm(shmid);
    return 0;
}