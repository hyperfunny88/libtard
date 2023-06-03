#include "libtard.c"
#include <stdio.h>

#define DIE() do { fputs("fail\n", stderr); return -1; } while (0)

int main(void)
{
	Lt lt;
	lt_make(&lt, 5, 0.97f);
	for (int i = 0; i < 100; ++i) {
		if (lt_add(&lt, i, i) != lt_find(&lt, i))
			DIE();
	}
	lt_rm(&lt, lt_find(&lt, 50));
	for (int i = 0; i < 100; ++i) {
		if (lt_find(&lt, i) == NULL && i != 50)
			DIE();
	}
	printf("%u/%u/%u\n", lt.sz, lt.lsz, lt.msz);
	lt_dstr(&lt);
	fputs("ok\n", stderr);
	return 0;
}
