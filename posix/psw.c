#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>

// gcc psw.c -o psw -lrt

/*封装打印出错函数*/
void sys_err(const char *str,int num){
    perror(str);
    exit(num);
}

int main(int argc,char *argv[]) {
    /* O_EXCL|O_CREAT, 若文件已经存在，则报错 */
    int fd = shm_open("hello.txt", O_RDWR|O_CREAT|O_EXCL, 0777);
    
    if(fd < 0){
        /* 直接打开文件读写 */
        fd = shm_open("hello.txt", O_RDWR, 0777);
        
    }else {
        /* 若为自己创建的文件，则为文件分配内存空间大小 */
        ftruncate(fd, 4096);
    }
    void *ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    puts("start writeing  data....");
    /* 写文件 */
    strcpy((char*)ptr,"hell world");
    puts("write over");
    getchar();
    shm_unlink("hello.txt");
    close(fd);
    return 0;
  }
