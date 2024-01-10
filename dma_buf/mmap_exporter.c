int main(int argc, char *argv[])
{
	int fd;

	fd = open("/dev/exporter", O_RDONLY);

	char *str = mmap(NULL, 4096, PROT_READ, MAP_SHARED, fd, 0);
	printf("read from /dev/exporter mmap: %s\n", str);

	close(fd);

	return 0;
}
