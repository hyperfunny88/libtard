#include "libtard.c"
#include <stdio.h>

#define DIE() do { fputs("fail\n", stderr); return 1; } while (0)

int main(void)
{
	Lt lt;
	enum {N = 1000000};
	lt_make(&lt, N / 2, 0.98f);
	for (u32 i = 0; i < N; ++i) {
		if (lt_add(&lt, i, i) != lt_find(&lt, i))
			DIE();
	}
	lt_rm(&lt, lt_find(&lt, 50));
	for (u32 i = 0; i < N; ++i) {
		if (lt_find(&lt, i) == NULL && i != 50)
			DIE();
	}
	fprintf(stderr,
		"%u/%u/%u (%u%%/%u%%)\n",
		lt.sz, lt.lsz, lt.msz, (u32)((uint64_t)100 * lt.sz / lt.msz),
		(u32)((uint64_t)100 * lt.lsz / lt.msz));
	lt_dstr(&lt);
	puts("ok");
	return 0;
}
