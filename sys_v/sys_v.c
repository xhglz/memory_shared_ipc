#include "sys_v.h"

static int commShm(int size,int flags) {
    // 用于标识系统的唯一IPC资源
    key_t key = ftok(PATHNAME, PROJ_ID);
    if(key < 0){
        perror("ftok");
        return -1;
    }
    // 如果共享内存不存在，创建共享内存，如果存在就打开共享内存
    // int shmget(key_t key, size_t size, int shmflg);
    // key: 共享内存的标识符
    // size: 共享内存的大小, 一般指定为4k的倍数（内存4k为一页）
    // shmflg: 共享内存的访问权限
    // - IPC_CREAT:创建新的共享内存。
    // - IPC_EXCL:与IPC_CREAT一同使用，表示如果要创建的共享内存已经存在，则返回错误。 
    // - IPC_NOWAIT:读写共享内存要求无法满足时，不阻塞0：如果是打开文件，即文件已存在，写0
    // 返回值：成功返回共享内存的标识符，失败返回-1
    int shmid = shmget(key, size, flags);
    if(shmid < 0){
        perror("shmget");
        return -2;
    }
    return shmid;
}

// 创建共享内存
int createShm(int size) {
    return commShm(size, IPC_CREAT | IPC_EXCL);
}

// 获取共享内存
int getShm(int size) {
    return commShm(size, IPC_CREAT);
}

// 销毁共享内存
int destoryShm(int shmid) {
    if(shmctl(shmid, IPC_RMID,NULL) < 0){
        perror("shmctl");
        return -3;
    }
}