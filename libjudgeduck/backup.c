#include <inc/lib.h>

extern union Fsipc fsipcbuf;
extern const char * const jd_error_string[MAXERROR];

void jd_backup(void *buf) {
	static_assert(sizeof(fsipcbuf) == PGSIZE);
	void *cur = buf;
	memcpy(cur, &fsipcbuf, PGSIZE); cur = (char *) cur + PGSIZE;
	memcpy(cur, &thisenv, sizeof(thisenv)); cur = (char *) cur + sizeof(thisenv);
	memcpy(cur, &binaryname, sizeof(binaryname)); cur = (char *) cur + sizeof(binaryname);
	memcpy(cur, &devfile, sizeof(devfile)); cur = (char *) cur + sizeof(devfile);
	memcpy(cur, &devcons, sizeof(devcons)); cur = (char *) cur + sizeof(devcons);
	memcpy(cur, &jd_error_string, sizeof(jd_error_string)); cur = (char *) cur + sizeof(jd_error_string);
}

void jd_restore(const void *buf) {
	const void *cur = buf;
	memcpy((void *) &fsipcbuf, cur, PGSIZE); cur = (char *) cur + PGSIZE;
	memcpy((void *) &thisenv, cur, sizeof(thisenv)); cur = (char *) cur + sizeof(thisenv);
	memcpy((void *) &binaryname, cur, sizeof(binaryname)); cur = (char *) cur + sizeof(binaryname);
	memcpy((void *) &devfile, cur, sizeof(devfile)); cur = (char *) cur + sizeof(devfile);
	memcpy((void *) &devcons, cur, sizeof(devcons)); cur = (char *) cur + sizeof(devcons);
	memcpy((void *) &jd_error_string, cur, sizeof(jd_error_string)); cur = (char *) cur + sizeof(jd_error_string);
}
