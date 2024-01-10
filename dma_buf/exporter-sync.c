#include <linux/dma-buf.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

struct dma_buf *dmabuf_exported;
EXPORT_SYMBOL(dmabuf_exported);

...

static int exporter_begin_cpu_access(struct dma_buf *dmabuf,
				      enum dma_data_direction dir)
{
	dma_addr_t dma_addr = virt_to_phys(dmabuf->priv);

	dma_sync_single_for_cpu(NULL, dma_addr, PAGE_SIZE, dir);

	return 0;
}

static int exporter_end_cpu_access(struct dma_buf *dmabuf,
				enum dma_data_direction dir)
{
	dma_addr_t dma_addr = virt_to_phys(dmabuf->priv);

	dma_sync_single_for_device(NULL, dma_addr, PAGE_SIZE, dir);

	return 0;
}

static const struct dma_buf_ops exp_dmabuf_ops = {
	...
	.begin_cpu_access = exporter_begin_cpu_access,
	.end_cpu_access = exporter_end_cpu_access,
};

static struct dma_buf *exporter_alloc_page(void)
{
	DEFINE_DMA_BUF_EXPORT_INFO(exp_info);
	struct dma_buf *dmabuf;
	void *vaddr;

	vaddr = kzalloc(PAGE_SIZE, GFP_KERNEL);

	exp_info.ops = &exp_dmabuf_ops;
	exp_info.size = PAGE_SIZE;
	exp_info.flags = O_CLOEXEC;
	exp_info.priv = vaddr;

	dmabuf = dma_buf_export(&exp_info);

	sprintf(vaddr, "hello world!");

	return dmabuf;
}

static long exporter_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int fd = dma_buf_fd(dmabuf_exported, O_CLOEXEC);
	return copy_to_user((int __user *)arg, &fd, sizeof(fd));
}
 
static struct file_operations exporter_fops = {
	.owner		= THIS_MODULE,
	.unlocked_ioctl	= exporter_ioctl,
};
 
static struct miscdevice mdev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "exporter",
	.fops = &exporter_fops,
};
 
static int __init exporter_init(void)
{
	dmabuf_exported = exporter_alloc_page();
	return misc_register(&mdev);
}

module_init(exporter_init);
