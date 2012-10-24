/*
 * include/memory.h
 * Copyright (C) 2003 Alexander Blessing
 */

#ifndef _INCLUDE_MEMORY_H_
#define _INCLUDE_MEMORY_H_

#include <mm/paging.h>

/* prototypes */
void *phys_alloc(void);
void phys_free(void *);
void init_phys_alloc(void);
void init_paging(void);
void init_vmm(void);
void init_mm(void);

void flush_tlb(void);
void invlpg(addr_t);

inline int bitmap_is_bit_set(unsigned long *, unsigned long);
inline int bitmap_is_bit_clear(unsigned long *, unsigned long);
inline void bitmap_set_bit(unsigned long *, unsigned long);
inline void bitmap_clear_bit(unsigned long *, unsigned long);
inline int bitmap_test_and_set_bit(unsigned long *, unsigned long);
inline int bitmap_test_and_clear_bit(unsigned long *, unsigned long);
inline unsigned long bitmap_find_first_zero_bit(unsigned long *, unsigned long);
inline unsigned long bitmap_search(unsigned long *, unsigned long, 
			        unsigned long, unsigned long);

#endif
