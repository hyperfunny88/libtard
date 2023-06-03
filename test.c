#include "libtard.c"
#include <stdio.h>

#define DIE() do { fputs("fail\n", stderr); return -1; } while (0)

int main(void)
{
	Lt lt;
	lt_make(&lt, 42069, 0.99f);
	for (u32 i = 0; i < 42069; ++i) {
		if (lt_add(&lt, i, i) != lt_find(&lt, i))
			DIE();
	}
	lt_rm(&lt, lt_find(&lt, 50));
	for (u32 i = 0; i < 42069; ++i) {
		if (lt_find(&lt, i) == NULL && i != 50)
			DIE();
	}
	lt_dstr(&lt);
	fprintf(stderr,
		"%u/%u/%u (%u%%/%u%%)\n"
		"ok\n",
		lt.sz, lt.lsz, lt.msz, 100 * lt.sz / lt.msz,
		100 * lt.lsz / lt.msz);
	return 0;
}
