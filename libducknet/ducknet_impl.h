#ifndef __DUCKNET_IMPL_H__
#define __DUCKNET_IMPL_H__

#include <ducknet_types.h>
#include <ducknet_ether.h>
#include <ducknet_ipv4.h>

#ifdef __cplusplus
extern "C" {
#endif

// === protocols ===

const int MAX_MTU = 1500;

extern char ducknet_sendbuf[MAX_MTU + 64];

extern DucknetMACAddress ducknet_mac;
extern int ducknet_MTU;

extern DucknetIPv4Address ducknet_ip;

// === time ===

extern ducknet_time_t ducknet_currenttime;

// === string operations ===

int strlen(const char *s);
char * strcpy(char *dst, const char *src);
char * strncpy(char *dst, const char *src, unsigned size);
char * strcat(char *dst, const char *src);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, unsigned size);
char * strchr(const char *s, char c);

void * memset(void *dst, int c, unsigned len);
void * memcpy(void *dst, const void *src, unsigned len);
void * memmove(void *dst, const void *src, unsigned len);
int memcmp(const void *s1, const void *s2, unsigned len);

#ifdef __cplusplus
}
#endif

#endif
