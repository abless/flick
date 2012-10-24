/*
 * mm/paging.c
 * Copyright (C) 2003 Alexander Blessing
 */ 

#include <stdio.h>
#include <memory.h>
#include <kernel/kernel.h>

#define PDIR_SELFMAPPING_ENTRY 1023

extern addr_t kernel_end;	/* end of kernel (physical) */

addr_t *pdir;

void page_fault(void)
{
	addr_t addr;
	
	/* a page fault has occured, that means that someone is trying to 
	 * access a not present page
	 * we could make that page present, but because of safety reasons, we
	 * will panic: memory that isn't present is not supposed to be accessed
	 */
	__asm__ ("movl %%cr2, %0\n" : "=r" (addr));	/* get invalid address */
	panic("page fault at virtual address %i\n", addr);
}

/* paging setup routine */
void init_paging(void)
{
	int i;

	printf("Setting up Paging ...\n");
	pdir = (addr_t *) PAGE_ALIGN((addr_t) &kernel_end + 1); 

	/* set up the page directory entries */
	for (i = 0; i < 1024; i++) 
		pdir[i] = PRIV_KERNEL | PRIV_RW;	/* since we set this to non-present, the address does not matter */

	/* use selfmapping: pdir can be handled as a seperate ptable */
	pdir[PDIR_SELFMAPPING_ENTRY] = (addr_t) pdir | PRIV_KERNEL | PRIV_RW | PRIV_PRES;	/* flags: present, supervisor level, read/write */ 

	/* only a page directory is set up
	 * next, phys_alloc will be initialized and then the
	 * kernel will be mapped
	 */
}

/* return page size */
inline unsigned long getpagesize(void)
{
	return PAGE_SIZE;
}
