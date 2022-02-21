/* sort_r as a portable alternative to qsort_r */
/* (C) 2022, bel2125, MIT license */

#ifndef sort_r_included
#define sort_r_included

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

typedef int (*sort_r_compare)(const void *data1, const void *data2, void *userarg);

void sort_r(void *data,
	size_t elemcount,
	size_t elemsize,
	sort_r_compare compfunc,
	void *userarg);

#ifdef __cplusplus
}
#endif

#endif /* sort_r_included */

