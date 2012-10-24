/*
 * kernel/misc.c
 * Copyright (C) 2002 Alexander Blessing
 * 
 * contains useful functions
 */

#include <types.h>

inline void flush_tlb(void)
{
	__asm__ __volatile__
		("movl %%cr3, %%eax; movl %%eax, %%cr3" ::: "eax");
}

inline void invlpg(addr_t *virt)
{
	__asm__ __volatile__
		("invlpg (%0)" :: "r" (virt));
}
