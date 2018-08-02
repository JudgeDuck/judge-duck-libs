#include <locale.h>

extern "C" void __stack_chk_fail_local() {}

static locale_t curloc;
extern "C" locale_t __uselocale(locale_t newloc) {
	locale_t ret = curloc;
	curloc = newloc;
	return curloc;
}

