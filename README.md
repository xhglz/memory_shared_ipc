# 共享内存通信
参考：[最好的共享内存(Linux共享内存最透彻的一篇)](https://zhuanlan.zhihu.com/p/574398364)

## 1 基于传统SYS V的共享内存
略
## 2 基于POSIX mmap文件映射实现共享内存
编译
```shell
gcc psr.c -o psr -lrt
gcc psw.c -o psw -lrt
```
运行
```shell
# 终端1
./psw
# 终端2
./psr
```
## 3 通过memfd_create()和fd跨进程共享实现共享内存
编译
```shell
gcc -o send send.c
gcc -o recv recv.c
```
运行
```shell
# 终端1
./recv
# 终端2
./send
```
## 4 多媒体、图形领域广泛使用的基于dma-buf的共享内存
- [dma-buf 由浅入深](https://blog.csdn.net/hexiaolong2009/article/details/102596744)