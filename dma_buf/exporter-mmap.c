#include <linux/dma-buf.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/uaccess.h>

struct dma_buf *dmabuf_exported;
EXPORT_SYMBOL(dmabuf_exported);

static int exporter_mmap(struct dma_buf *dmabuf, struct vm_area_struct *vma)
{
	void *vaddr = dmabuf->priv;

	return remap_pfn_range(vma, vma->vm_start, virt_to_pfn(vaddr),
				PAGE_SIZE, vma->vm_page_prot);
}

...
static const struct dma_buf_ops exp_dmabuf_ops = {
	...
	.mmap = exporter_mmap,
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

static int exporter_misc_mmap(struct file *file, struct vm_area_struct *vma)
{
	return dma_buf_mmap(dmabuf_exported, vma, 0);
}

static struct file_operations exporter_fops = {
	.owner	= THIS_MODULE,
	.mmap	= exporter_misc_mmap,
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

static void __exit exporter_exit(void)
{
	misc_deregister(&mdev);
}

module_init(exporter_init);
module_exit(exporter_exit);
