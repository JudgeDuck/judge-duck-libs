#define _GNU_SOURCE

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdbool.h>
#include <errno.h>

extern bool _pigeon_filter(const char *path);

#define PATH_MAX 4096

int open(const char *path, int flags, ...) {
	//fprintf(stderr, "[open] [%s] [0x%x]\n", path, flags);
	char filename[PATH_MAX + 1];
	int tmp_len = 0;
	if ((tmp_len = readlink(path, filename, PATH_MAX)) > 0) {
		filename[tmp_len] = 0;
		//fprintf(stderr, "[%s] ", filename);
		if (!_pigeon_filter(filename)) {
			errno = ENOENT;
			return -1;
		}
	} else {
		//fprintf(stderr, "[readlink failed, path=%s]", path);
		errno = 0;
		if (!_pigeon_filter(path)) {
			errno = ENOENT;
			return -1;
		}
	}
	int fd = ((int (*)(const char *, int)) dlsym(RTLD_NEXT, "open")) (path, flags);
	if (fd >= 0) {
		int flags = fcntl(fd, F_GETFL, 0);
		int ret = fcntl(fd, F_SETFL, flags | O_NONBLOCK);
		//fprintf(stderr, "fcntl ret = %d\n", ret);
	}
	return fd;
}

ssize_t read(int fd, void *buf, size_t len) {
	//fprintf(stderr, "[read] [%d] [%p] [%lu] ... ", fd, buf, len);
	int flags = fcntl(fd, F_GETFL, 0);
	fcntl(fd, F_SETFL, flags | O_NONBLOCK);
	char filename[PATH_MAX + 1];
	char fd_name[PATH_MAX];
	sprintf(fd_name, "/proc/self/fd/%d", fd);
	int tmp_len = 0;
	if ((tmp_len = readlink(fd_name, filename, PATH_MAX)) > 0) {
		filename[tmp_len] = 0;
		//fprintf(stderr, "[%s] ", filename);
		if (!_pigeon_filter(filename)) {
			errno = ENOENT;
			return -1;
		}
	} else {
		errno = 0;
	}
	
	ssize_t ret = ((ssize_t (*)(int, void *, size_t)) dlsym(RTLD_NEXT, "read")) (fd, buf, len);
	//fprintf(stderr, "= %lu\n", ret);
	return ret;
}
