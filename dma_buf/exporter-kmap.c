// exporter 通过 kzalloc 分配了一个 PAGE 大小的物理连续 buffer，并向该 buffer 写入了“hello world!” 字符串；

#include <linux/dma-buf.h>
#include <linux/module.h>
#include <linux/slab.h>

struct dma_buf *dmabuf_exported;
EXPORT_SYMBOL(dmabuf_exported);

static void *exporter_kmap(struct dma_buf *dmabuf, unsigned long page_num)
{
	return dmabuf->priv;
}

static void *exporter_kmap_atomic(struct dma_buf *dmabuf, unsigned long page_num)
{
	return dmabuf->priv;
}

static void *exporter_vmap(struct dma_buf *dmabuf)
{
	return dmabuf->priv;
}

static void exporter_release(struct dma_buf *dmabuf)
{
	kfree(dmabuf->priv);
}

...

static const struct dma_buf_ops exp_dmabuf_ops = {
	.map = exporter_kmap,
	.map_atomic = exporter_kmap_atomic,
	.vmap = exporter_vmap,
	.release = exporter_release,
	...
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

static int __init exporter_init(void)
{
	dmabuf_exported = exporter_alloc_page();
	return 0;
}

module_init(exporter_init);
