int main(int argc, char *argv[])
{
	int fd;
	int dmabuf_fd = 0;

	fd = open("/dev/exporter", O_RDONLY);
	ioctl(fd, 0, &dmabuf_fd);
	close(fd);

	fd = open("/dev/importer", O_RDONLY);
	ioctl(fd, 0, &dmabuf_fd);
	close(fd);

	return 0;
}
