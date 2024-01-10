#include <unistd.h>
#include "sys_v.h"

// 基于传统SYS V的共享内存
// gcc -o client sys_v_client.c sys_v.c
int main() {
    // 创建共享内存
    int shmid = getShm(4096);
    // 将共享内存进行挂接
    char* addr = (char*)shmat(shmid, NULL, 0);
    int i = 0;
    while(i < 26) {
        addr[i] = 'A';
        i++;
        addr[i] = 0;
        sleep(1);
    }
    // 取挂接
    shmdt(addr);
    return 0;
}