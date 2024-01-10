// importer 驱动通过 extern 关键字导入了 exporter 的 dma-buf，并通过 dma_buf_kmap()、dma_buf_vmap() 函数读取该 buffer 的内容并输出到终端显示。
#include <linux/dma-buf.h>
#include <linux/module.h>
#include <linux/slab.h>

extern struct dma_buf *dmabuf_exported;

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

static int __init importer_init(void)
{
	return importer_test(dmabuf_exported);
}

module_init(importer_init);
