#include <taskduck.h>
#include <inc/lib.h>

#undef NULL
#define NULL 0

static void do_judge(TaskDuck *td, void *eip);

void TaskDuck::judge_using_wrapper(void (*wrapper)()) {
	do_judge(this, (void *) wrapper);
}

namespace judgeduck {
	void *malloc_start;
	
	char *stdin_content;
	int stdin_size;
	
	char *stdout_content;
	int stdout_max_size;
	int stdout_size;
	
	volatile unsigned *start_signal;
	volatile unsigned *contestant_ready;
	volatile unsigned *contestant_done;
	
	char judge_signal_page[PGSIZE] __attribute__((aligned(PGSIZE)));
}

static void read_file(TaskDuck *td, const char *filename, char *&content, int &size) {
	if (filename == NULL) {
		content = NULL;
		size = 0;
	} else {
		int fd = jd_open(filename, O_RDONLY | O_CREAT);
		struct Stat stat;
		jd_fstat(fd, &stat);
		// jd_cprintf("[libtaskduck] [%s] size = %d\n", filename, stat.st_size);
		content = (char *) td->malloc(stat.st_size);
		int remain = stat.st_size;
		size = 0;
		while (remain) {
			int tmp = jd_read(fd, content + size, remain);
			if (tmp <= 0) {
				break;
			}
			size += tmp;
			remain -= tmp;
		}
	}
}

static void init_judgeduck(TaskDuck *td) {
	extern int ebss;
	
	// Map the first metadata page
	sys_map_judge_pages(td->judge_pages, 0, 2 * PGSIZE);
	
	// Map the judge signal page
	sys_page_map(0, td->judge_pages, 0, judgeduck::judge_signal_page, PTE_U | PTE_P | PTE_W);
	
	// Read the input info
	unsigned input_pos = ((unsigned *) td->judge_pages)[PGSIZE / 4 + 0];
	unsigned input_len = ((unsigned *) td->judge_pages)[PGSIZE / 4 + 1];
	sys_map_judge_pages(td->judge_pages + input_pos, input_pos, ROUNDUP(input_len, PGSIZE));
	td->stdin_content = td->judge_pages + input_pos;
	td->stdin_size = input_len;
	
	// Setup signals
	judgeduck::start_signal = (volatile unsigned *) (judgeduck::judge_signal_page + 0x100);
	judgeduck::contestant_ready = judgeduck::start_signal + 1;
	judgeduck::contestant_done = judgeduck::start_signal + 2;
	
	// read_file(td, td->input_filename, td->stdin_content, td->stdin_size);
	judgeduck::stdin_content = td->stdin_content;
	judgeduck::stdin_size = td->stdin_size;
	
	int n_output_pages = ROUNDUP(td->max_output_size, PGSIZE) / PGSIZE;
	td->stdout_content = (char *) &ebss;
	td->stdout_max_size = td->max_output_size;
	judgeduck::stdout_content = td->stdout_content;
	judgeduck::stdout_max_size = td->stdout_max_size;
	judgeduck::stdout_size = 0;
	
	judgeduck::malloc_start = (char *) &ebss + n_output_pages * PGSIZE;

	td->pre_alloc_memory();
	
	// jd_cprintf("init done\n");
}

//int arr[550 * (1024 / 4) << 10];

static void finish_judgeduck(TaskDuck *td) {
	// jd_cprintf("finish begin\n");
	int size = judgeduck::stdout_size;
	if (size < 0) {
		size = 0;
	} else if (size > td->stdout_max_size) {
		size = td->stdout_max_size;
	}
	td->stdout_size = size;
	
	// Write stdout metadata
	sys_map_judge_pages(td->judge_pages + PGSIZE * 3, PGSIZE * 3, PGSIZE);
	* (volatile unsigned *) (td->judge_pages + PGSIZE * 3) = (unsigned) size;
	
	// Map the second metadata page
	sys_map_judge_pages(td->judge_pages + PGSIZE * 2, PGSIZE * 2, PGSIZE);
	
	// Read the input info
	unsigned answer_pos = ((unsigned *) td->judge_pages)[PGSIZE * 2 / 4];
	unsigned answer_len = ((unsigned *) td->judge_pages)[PGSIZE * 2 / 4 + 1];
	sys_map_judge_pages(td->judge_pages + answer_pos, answer_pos, ROUNDUP(answer_len, PGSIZE));
	td->answer_content = td->judge_pages + answer_pos;
	td->answer_size = answer_len;
	
	// read_file(td, td->answer_filename, td->answer_content, td->answer_size);
}

static void *real_eip;

extern "C" {
	extern void exit(int);
}

static void judge_wrapper() {
	extern void libstdduck_init();
	extern void libstdduck_fini();
	
	// Wait for the start signal
	*judgeduck::contestant_ready = 1;
	while (*judgeduck::start_signal == 0) __asm__ volatile("pause");
	
	libstdduck_init();
	((void (*)()) real_eip)();
	exit(0);
}

static void do_judge(TaskDuck *td, void *eip) {
	struct JudgeParams prm;
	memset(&prm, 0, sizeof(prm));
	prm.ns = td->time_ns;
	prm.kb = td->mem_kb;
	#ifndef JD_OLD_MEMORY_LIMIT
		prm.esp = (char *) 0x10000000 + td->mem_kb_hard * 1024;
	#else
		prm.kb = td->mem_kb_hard;
	#endif
	prm.syscall_enabled[SYS_quit_judge] = 1;
	
	init_judgeduck(td);
	
	char *backup_buf = (char *) td->malloc(JD_BACKUP_SIZE);
	jd_backup(backup_buf);
	
	real_eip = eip;
	sys_enter_judge((void *) judge_wrapper, &prm);
	
	jd_restore(backup_buf);
	
	finish_judgeduck(td);
	
	if (td->cmp) {
		td->cmp_result = td->cmp(
			td->stdin_content, td->stdin_size,
			td->stdout_content, td->stdout_size,
			td->answer_content, td->answer_size
		);
	}
}
