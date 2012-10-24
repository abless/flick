/*
 * include/mm/paging.h
 * Copyright (C) 2002 Alexander Blessing
 */

#ifndef _INCLUDE_MM_PAGING_H_
#define _INCLUDE_MM_PAGING_H_

#include <types.h>

#define PAGES_PER_SUPERPAGE 1024
#define PAGE_SIZE 4096

#define PAGE_ALIGN(addr) (((addr) + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1))

/* flags */
#define PRIV_PRES 0x001	/* present bit */
#define PRIV_RW 0x002	/* read/write privilege */
#define PRIV_KERNEL 0x000	/* supervisor mode */
#define PRIV_USER 0x004	/* user mode */
#define PRIV_RO 0x000	/* read only */

#define IS_PRESENT 0x001

/* bitmap operation macros */
#define FIRST_ZERO_BIT 0

#define ADDR_TO_PAGE(paddr) (paddr / PAGE_SIZE)
#define PAGE_TO_SUPERPAGE(page) (page / PAGES_PER_SUPERPAGE)

#define NUM_PAGES(size) ((size + PAGE_SIZE - 1) / PAGE_SIZE)
#define NUM_SUPERPAGES(pmem_size) \
	((NUM_PAGES(pmem_size) + PAGES_PER_SUPERPAGE - 1) / PAGES_PER_SUPERPAGE)
#define SUPERPAGE_BITMAP_SIZE(pmem_size) ((NUM_SUPERPAGES(pmem_size) + 7) / 8)
#define PAGE_BITMAP_SIZE(pmem_size) ((NUM_PAGES(pmem_size) + 7) / 8)	

#define BITS_PER_ELEMENT (sizeof(unsigned long) * 8)
#define BITMAP_MASK(bit) (1 << (bit % BITS_PER_ELEMENT))
#define BITMAP_OFFSET(bit) (bit / BITS_PER_ELEMENT)

/* TLB */
#define FLUSH 10
#define NO_FLUSH 11


/* prototypes */
void load_pdir(addr_t *pdir);
void enable_paging(void);

#endif
