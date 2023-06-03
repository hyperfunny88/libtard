#include "libtard.c"
#include <stdio.h>

int main(void)
{
	Lt lt;
	lt_make(&lt, 100, 0.85f);
	for (int i = 0 ;i < 100; ++i) {
		printf("%u/%u, %u\n", lt.sz, lt.lsz, lt.msz);
		printf("%p\n", lt_add(&lt, i, i));
		printf("%p\n", lt_find(&lt, i));
	}
	lt_rm(&lt, lt_find(&lt, 50));
	lt_dstr(&lt);
}
