#include <linux/dma-buf.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/uaccess.h>

static int importer_test(struct dma_buf *dmabuf)
{
	void *vaddr;

	vaddr = dma_buf_kmap(dmabuf, 0);
	pr_info("read from dmabuf kmap: %s\n", (char *)vaddr);
	dma_buf_kunmap(dmabuf, 0, vaddr);

	vaddr = dma_buf_vmap(dmabuf);
	pr_info("read from dmabuf vmap: %s\n", (char *)vaddr);
	dma_buf_vunmap(dmabuf, vaddr);

	return 0;
}

static long importer_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int fd;
	struct dma_buf *dmabuf;

	copy_from_user(&fd, (void __user *)arg, sizeof(int));

	dmabuf = dma_buf_get(fd);
	importer_test(dmabuf);
	dma_buf_put(dmabuf);

	return 0;
}
 
static struct file_operations importer_fops = {
	.owner	= THIS_MODULE,
	.unlocked_ioctl	= importer_ioctl,
};
 
static struct miscdevice mdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "importer",
	.fops = &importer_fops,
};
 
static int __init importer_init(void)
{
	return misc_register(&mdev);
}

static void __exit importer_exit(void)
{
	misc_deregister(&mdev);
}

module_init(importer_init);
module_exit(importer_exit);

