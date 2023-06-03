/* Copyright (C) 2023 hyperfunny88
 * SPDX-License-Identifier: ISC */

#include "libtard.h"
#include <string.h>

typedef uint32_t u32;
typedef uint8_t u8;

static u32 align(u32 v)
{
	u32 a = 16, k = a - 1;
	return (v + k) & ~k;
}

static void seterr(Lt *lt)
{
	lt->msz = ~0u;
}

static int make(Lt *lt, u32 lsz, u32 msz)
{
	u32 ksz = sizeof(*lt->k) * msz,
	    vsz = sizeof(*lt->v) * msz, vo = ksz,
	    osz = sizeof(*lt->o) * msz, oo = vo + vsz;
	void *a = LT_ALLOC(ksz + vsz + osz);
	if (!a) {
		seterr(lt);
		return ~0u;
	}
	*lt = (Lt){
		.k = (void*)a,
		.v = (void*)(a + vo),
		.o = (void*)(a + oo),
		.sz = 0,
		.lsz = lsz,
		.msz = msz
	};
	memset(lt->o, 0, osz);
	return 1;
}

int lt_make(Lt *lt, u32 sz, float lf)
{
	sz = align((u32)(sz / lf));
	return make(lt, sz * lf, sz);
}

void lt_dstr(Lt *lt)
{
	LT_FREE(lt->k); /* k is base of alloc */
}

static u32 pos(const Lt *lt, u32 k)
{
	uint64_t M = 0xBEA225F9EB34556D, v = k;
	v ^= v >> 32; v *= M;
	v ^= v >> 29; v *= M;
	v ^= v >> 32; v *= M;
	v ^= v >> 29;
	return (uint64_t)(u32)v * lt->msz >> 32; /* [0, 2^32) -> [0, n) */
}

static u32 fwd(const Lt *lt, u32 p)
{
	return ++p != lt->msz ? p : 0;
}

static u32 bwd(const Lt *ht, u32 p)
{
	return p-- ? p : ht->msz - 1;
}

static int rehash(Lt *lt)
{
	Lt new;
	if (!make(&new, lt->lsz * 2, lt->msz * 2))
		return 0;
	for (u32 i = 0; i < lt->msz && lt->sz; ++i) {
		if (!lt->o[i])
			continue;
		if (lt_add(&new, lt->k[i], lt->v[i]) == NULL)
			return 0;
		--lt->sz;
	}
	lt_dstr(lt);
	*lt = new;
	return 1;
}

static u32 *add(Lt *lt, u32 k, u32 v)
{
	if (lt->sz == lt->lsz)
		return NULL;
	u32 p = pos(lt, k), o = 1;
	for (; o <= lt->o[p]; p = fwd(lt, p), ++o);
	if (o >= 0xFF)
		return NULL;
	u32 t = p;
	for (; lt->o[p]; p = fwd(lt, p));
	for (u32 b; p != t; p = b) {
		b = bwd(lt, p);
		lt->k[p] = lt->k[b];
		lt->v[p] = lt->v[b];
		lt->o[p] = lt->o[b] + 1;
		if (lt->o[p] == 0xFF) {
			lt->o[p] = 0; /* 'erase' entry */
			return NULL;
		}
	}
	++lt->sz;
	lt->k[p] = k;
	lt->v[p] = v;
	lt->o[p] = (u8)o;
	return &lt->v[p];
}

u32 *lt_add(Lt *lt, u32 k, u32 v)
{
	u32 *r;
	while ((r = add(lt, k, v)) == NULL) {
		if (lt_err(lt))
			return NULL;
		rehash(lt);
	}
	return r;
}

void lt_rm(Lt *lt, uint32_t *v)
{
	--lt->sz;
	u32 p = v - lt->v;
	for (u32 f; lt->o[f] > 1; p = f) {
		f = fwd(lt, p);
		lt->k[p] = lt->k[f];
		lt->v[p] = lt->v[f];
		lt->o[p] = lt->o[f] - 1;
	}
	lt->o[p] = 0;
}

u32 *lt_find(const Lt *lt, u32 k)
{
	u32 p = pos(lt, k), o = 1;
	for (; o < lt->o[p]; p = fwd(lt, p), ++o);
	for (; o == lt->o[p]; p = fwd(lt, p), ++o) {
		if (lt->k[p] == k)
			return &lt->v[p];
	}
	return NULL;
}

int lt_get(const Lt *lt, u32 p, u32 *k, u32 *v)
{
	if (!lt->o[p])
		return 0;
	*k = lt->k[p];
	*v = lt->v[p];
	return 1;
}