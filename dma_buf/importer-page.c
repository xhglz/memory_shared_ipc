#include <linux/dma-buf.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

static int importer_test1(struct dma_buf *dmabuf)
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

static int importer_test2(struct dma_buf *dmabuf)
{
	struct dma_buf_attachment *attachment;
	struct sg_table *table;
	struct device *dev;
	unsigned int reg_addr, reg_size;

	dev = kzalloc(sizeof(*dev), GFP_KERNEL);
	dev_set_name(dev, "importer");

	attachment = dma_buf_attach(dmabuf, dev);
	table = dma_buf_map_attachment(attachment, DMA_BIDIRECTIONAL);

	reg_addr = sg_dma_address(table->sgl);
	reg_size = sg_dma_len(table->sgl);
	pr_info("reg_addr = 0x%08x, reg_size = 0x%08x\n", reg_addr, reg_size);

	dma_buf_unmap_attachment(attachment, table, DMA_BIDIRECTIONAL);
	dma_buf_detach(dmabuf, attachment);

	return 0;
}

static long importer_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int fd;
	struct dma_buf *dmabuf;

	if (copy_from_user(&fd, (void __user *)arg, sizeof(int)))
		return -EFAULT;

	dmabuf = dma_buf_get(fd);

	importer_test1(dmabuf);
	importer_test2(dmabuf);

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
