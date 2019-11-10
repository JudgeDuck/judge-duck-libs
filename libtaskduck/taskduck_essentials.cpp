#include <taskduck.h>
#include <inc/lib.h>

#undef NULL
#define NULL 0

// Defaults:
// 1s, 256MB, 400MB, 64MB, input.txt, answer.txt, multiline_cmp
TaskDuck::TaskDuck() {
	this->time_ns = 1000 * 1000 * 1000;
	this->mem_kb = 256 * 1024;
	this->mem_kb_hard = 400 * 1024;
	this->max_output_size = 64 * 1024 * 1024;
	this->input_filename = "input.txt";
	this->answer_filename = "answer.txt";
	this->malloc_end = (void *) 0xd0000000;
	this->cmp = TaskDuck::multiline_cmp;
	this->memory_allocated = false;
	this->message = NULL;
	this->judge_pages = (char *) this->malloc(JUDGE_PAGES_SIZE, false);
}

void TaskDuck::set_time_limit(long long time_ns) {
	this->time_ns = time_ns;
}

void TaskDuck::set_memory_limit(int mem_kb) {
	this->mem_kb = mem_kb;
}

void TaskDuck::set_memory_hard_limit(int mem_kb) {
	this->mem_kb_hard = mem_kb;
}

void TaskDuck::set_max_output_size(int size) {
	this->max_output_size = size;
}

void TaskDuck::set_input_file(const char *filename) {
	this->input_filename = filename;
}

void TaskDuck::set_answer_file(const char *filename) {
	this->answer_filename = filename;
}

void TaskDuck::set_comparator(bool (*cmp)(const char *, int, const char *, int, const char *, int)) {
	this->cmp = cmp;
}

void quit_judge() {
	sys_quit_judge();
}

namespace judgeduck {
	extern char *malloc_start;
	extern int stdout_max_size;
}

void TaskDuck::pre_alloc_memory() {
	if (this->memory_allocated) {
		return;
	}
	this->memory_allocated = true;
	
	extern int ebss;
	char *mem_end = ROUNDUP((char *) 0x10000000 + this->mem_kb_hard * 1024, PGSIZE);
	if (mem_end <= judgeduck::malloc_start) {
		mem_end = judgeduck::malloc_start;
		this->mem_kb_hard = (judgeduck::malloc_start - (char *) &ebss) / 1024;
	}
	
	// map stdout pages
	int stdout_pages_len = judgeduck::malloc_start - (char *) &ebss;
	int stdout_pages_n = stdout_pages_len / PGSIZE;
	if (sys_map_judge_pages(&ebss, 4 * PGSIZE, stdout_pages_len) != stdout_pages_n) {
		jd_exit();
	}
	memset(&ebss, 0, stdout_pages_len);
	
	jd_cprintf("max stdout size = %d !!!!!!\n", judgeduck::stdout_max_size);
	
	// alloc heap memory
	if (sys_page_alloc_range(0, judgeduck::malloc_start, mem_end, PTE_P | PTE_U | PTE_W) < 0) {
		jd_exit();
	}
}
