int main(int argc, char *argv[])
{
	int fd;
	int dmabuf_fd = 0;

	fd = open("/dev/exporter", O_RDONLY);
	ioctl(fd, 0, &dmabuf_fd);
	close(fd);

	char *str = mmap(NULL, 4096, PROT_READ, MAP_SHARED, dmabuf_fd, 0);
	printf("read from dmabuf mmap: %s\n", str);

	return 0;
}
