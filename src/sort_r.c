/* sort_r as a portable alternative to qsort_r */
/* (C) 2022, bel2125, MIT license */

#include "sort_r.h"
#include <alloca.h>
#include <string.h>

void sort_r(void *data,
	size_t elemcount,
	size_t elemsize,
	sort_r_compare compfunc,
	void *userarg)
{
	/* ShellSort with gap sequence: https://oeis.org/A102549 */
	int A102549[9] = { 1, 4, 10, 23, 57, 132, 301, 701, 1750 };
	int Aidx, gap, i, j, k;
	void * tmp = alloca(elemsize);

	for (Aidx = 8; Aidx >= 0; Aidx--) {
		gap = A102549[Aidx];
		if (gap > elemcount / 2) {
			continue;
		}
		for (i = 0; i < gap; i++) {
			for (j = i; j < elemcount; j += gap) {
				/* tmp = data[j] */
				memcpy(tmp, (void *)((ptrdiff_t)data + elemsize * j), elemsize);

				for (k = j; k >= gap; k -= gap) {
					/* cmp = &data[k-gap] */
					void *cmp = (void *)((ptrdiff_t)data + elemsize * (k - gap));
					/* data[k-gap] > tmp */
					int cmpres = compfunc(cmp, tmp, userarg);
					if (cmpres > 0) {
						/* data[k] = data[k-gap] */
						memcpy((void *)((ptrdiff_t)data + elemsize * k), cmp, elemsize);
					} else {
						break;
					}
				}
				/* data[k] = tmp */
				memcpy((void *)((ptrdiff_t)data + elemsize * k), tmp, elemsize);
			}
		}
	}
}
