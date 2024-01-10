#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>

// gcc psr.c -o psr -lrt

void sys_err(const char *str,int num){
    perror(str);
    exit(num);
}

int main(int argc,char *argv[]) {
    int fd = shm_open("hello.txt", O_RDWR|O_CREAT|O_EXCL, 0777);
    if (fd < 0) {
        fd = shm_open("hello.txt", O_RDWR, 0777);
    } else {
        ftruncate(fd, 4096);
    }

    void *ptr = mmap(NULL, 4096, PROT_READ|PROT_WRITE,MAP_SHARED, fd, 0);

    puts("start reading data....");
    puts((char*)ptr);
    puts("read over");
    shm_unlink("hello.txt");
    close(fd);
    return 0;
  }