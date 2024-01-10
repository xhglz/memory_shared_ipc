#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/dma-buf.h>

int main(int argc, char *argv[])
{
	int fd;
	int dmabuf_fd = 0;
	struct dma_buf_sync sync = { 0 };

	fd = open("/dev/exporter", O_RDONLY);

	ioctl(fd, 0, &dmabuf_fd);
	close(fd);

	sync.flags = DMA_BUF_SYNC_READ | DMA_BUF_SYNC_START;
	ioctl(dmabuf_fd, DMA_BUF_IOCTL_SYNC, &sync);w

	char *str = mmap(NULL, 4096, PROT_READ, MAP_SHARED, dmabuf_fd, 0);
	printf("read from dmabuf mmap: %s\n", str);

	sync.flags = DMA_BUF_SYNC_READ | DMA_BUF_SYNC_END;
	ioctl(dmabuf_fd, DMA_BUF_IOCTL_SYNC, &sync);

	return 0;
}
