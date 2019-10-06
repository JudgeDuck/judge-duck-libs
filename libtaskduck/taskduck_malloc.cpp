#include <taskduck.h>
#include <inc/lib.h>

void * TaskDuck::malloc(size_t size, bool alloc) {
	size = ROUNDUP(size, PGSIZE);
	if (!alloc) {
		return this->malloc_end = (char *) this->malloc_end - size;
	}
	char *ret = (char *) this->malloc_end - size;
	if (sys_page_alloc_range(0, ret, this->malloc_end, PTE_P | PTE_U | PTE_W) < 0) {
		jd_exit();
	}
	this->malloc_end = ret;
	return ret;
}
