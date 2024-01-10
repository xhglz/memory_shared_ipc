#include <linux/dma-buf.h>
#include <linux/module.h>

static struct sg_table *exporter_map_dma_buf(struct dma_buf_attachment *attachment,
					 enum dma_data_direction dir)
{
	return NULL;
}

static void exporter_unmap_dma_buf(struct dma_buf_attachment *attachment,
			       struct sg_table *table,
			       enum dma_data_direction dir)
{
}

static void exporter_release(struct dma_buf *dmabuf)
{
}

static void *exporter_kmap_atomic(struct dma_buf *dmabuf, unsigned long page_num)
{
	return NULL;
}

static void *exporter_kmap(struct dma_buf *dmabuf, unsigned long page_num)
{
	return NULL;
}

static int exporter_mmap(struct dma_buf *dmabuf, struct vm_area_struct *vma)
{
	return -ENODEV;
}

static const struct dma_buf_ops exp_dmabuf_ops = {
	.map_dma_buf = exporter_map_dma_buf,
	.unmap_dma_buf = exporter_unmap_dma_buf,
	.release = exporter_release,
	.map_atomic = exporter_kmap_atomic,
	.map = exporter_kmap,
	.mmap = exporter_mmap,
};

static int __init exporter_init(void)
{
	DEFINE_DMA_BUF_EXPORT_INFO(exp_info);
	struct dma_buf *dmabuf;

	exp_info.ops = &exp_dmabuf_ops;
	exp_info.size = PAGE_SIZE;
	exp_info.flags = O_CLOEXEC;
	exp_info.priv = "null";

	dmabuf = dma_buf_export(&exp_info);

	return 0;
}

module_init(exporter_init);
