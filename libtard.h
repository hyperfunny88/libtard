/* simple and fast u32-u32 hash table
 * Copyright (C) 2023 hyperfunny88
 * SPDX-License-Identifier: ISC  */

#ifndef LIBTARD_H
#define LIBTARD_H

#include <stdint.h>

#ifndef LT_ALC
#include <stdlib.h>
#define LT_ALLOC(sz) malloc(sz)
#define LT_FREE(p) free(p)
#endif

typedef struct Lt {
	uint32_t *k, *v;
	uint8_t *o;
	uint32_t sz, lsz, msz;
	float lf;
} Lt;

int lt_make(Lt *lt, uint32_t sz, float lf);
void lt_dstr(Lt *lt);
uint32_t *lt_add(Lt *lt, uint32_t k, uint32_t v);
void lt_rm(Lt *lt, uint32_t *v);
uint32_t *lt_find(const Lt *lt, uint32_t k);
int lt_get(const Lt *lt, uint32_t p, uint32_t *k, uint32_t *v);

static inline uint32_t lt_sz(const Lt *lt)
{
	return lt->sz;
}

static inline uint32_t lt_lsz(const Lt *lt)
{
	return lt->lsz;
}

static inline uint32_t lt_msz(const Lt *lt)
{
	return lt->msz;
}

static inline int lt_err(const Lt *lt)
{
	return lt->msz == ~0u; /* for when malloc fails */
}

#endif
