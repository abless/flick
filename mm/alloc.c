/*
 * mm/alloc.c
 * Copyright (C) 2003 Alexander Blessing
 *
 * contains functions for allocating and freeing physical memory
 */

#include <stdio.h>
#include <string.h>
#include <memory.h>
#include <kernel/kernel.h>

extern unsigned long pmem_size;
extern addr_t *pdir;
extern addr_t kernel_end;

addr_t kspace_end;	/* end of kernel + bitmaps */
unsigned short *free_pages_in_superpage;
unsigned long superpage_bitmap_bits;
unsigned long *page_bitmap, *superpage_bitmap;

/* alloc_page_from_superpage()
 * allocates a page from a superpage
 */
void alloc_page_from_superpage(unsigned long page)
{
	unsigned long superpage;

	superpage = PAGE_TO_SUPERPAGE(page);
	/* do some checks first */
	if (free_pages_in_superpage[superpage] == 0)
		panic("superpage reserved, supposed to be free");

	free_pages_in_superpage[superpage]--;	/* decrement */
	if (free_pages_in_superpage[superpage] == 0) 
		bitmap_set_bit(superpage_bitmap, superpage);	/* mark superpage as reserved */
}

/* free_page_from_superpage()
 * deallocate a page from a superpage
 */
void free_page_from_superpage(unsigned long page)
{
	unsigned long superpage;

	superpage = PAGE_TO_SUPERPAGE(page);

	/* do some checks */
	if (free_pages_in_superpage[superpage] == PAGES_PER_SUPERPAGE)
		return;
	free_pages_in_superpage[superpage] += 1;

	/* if the superpage was reserved, now it is no longer */
	bitmap_clear_bit(superpage_bitmap, superpage);
}

/* allocate a page frame */
void __alloc_page_frame(unsigned long page)
{
	cli();	/* don't want interruption */
	
	if (bitmap_test_and_set_bit(page_bitmap, page))
		panic("page frame #%i (address %i) is not free", page, (page * PAGE_SIZE));
	alloc_page_from_superpage(page);
	
	sti();
}

void alloc_region(size_t start, size_t end)
{
	int page;
	
	if (start > pmem_size || end > pmem_size)
		panic("alloc_region(): start or end after pmem_size");

	start = PAGE_ALIGN(start);
	end = PAGE_ALIGN(end);
	
	for (page = ADDR_TO_PAGE(start); page < ADDR_TO_PAGE(end); page++)
		__alloc_page_frame(page);
}

/* allocate one page frame and return the (physical) start address */
void *phys_alloc(void)
{
	unsigned long superpage, page;
	
	/* search for free superpages */
	superpage = bitmap_search(superpage_bitmap, 0, superpage_bitmap_bits, FIRST_ZERO_BIT);
	
	if (superpage == superpage_bitmap_bits)	/* no free superpage found */
		return NULL;

	/* now find a free page in the superpage */
	page = bitmap_search(page_bitmap, PAGES_PER_SUPERPAGE * superpage, PAGES_PER_SUPERPAGE, FIRST_ZERO_BIT);
	if (page == PAGES_PER_SUPERPAGE)
		panic("no free page in free superpage");	/* there was supposed to be a free page */

	__alloc_page_frame(page);
	return (void *) (page * PAGE_SIZE);	/* that's it, return the (physical) start address */
}

/* free_page_frame()
 * deallocates one page frame
 */
void phys_free(void *ptr)
{
	addr_t paddr = (addr_t) ptr;

	/* ensure paddr is page aligned */
	paddr = PAGE_ALIGN(paddr);
	
	if (!bitmap_test_and_clear_bit(page_bitmap, paddr / PAGE_SIZE))
		printf("Paging: Page frame already free\n");
	
	free_page_from_superpage(ADDR_TO_PAGE(paddr));
}

void init_phys_alloc(void)
{
	int i;

	superpage_bitmap = (addr_t*) pdir + (1024 * 4) + 1;	/* superpage bitmap right after pdir */
	page_bitmap = superpage_bitmap + (SUPERPAGE_BITMAP_SIZE(pmem_size)) + 1;	/* page bitmap right after superpage_bitmap */
	free_pages_in_superpage =  (short*) page_bitmap + (PAGE_BITMAP_SIZE(pmem_size)) + 1;	/* free page count list right after page bitmap */ 
	
	superpage_bitmap_bits = NUM_SUPERPAGES(pmem_size); 
	kspace_end = (addr_t) (free_pages_in_superpage + superpage_bitmap_bits + 1);
	/* mark page frames in bitmaps as not reserved */
	memset(page_bitmap, 0, PAGE_BITMAP_SIZE(pmem_size));
	memset(superpage_bitmap, 0, SUPERPAGE_BITMAP_SIZE(pmem_size));

	/* and initialize the free page count list array */
	for (i = 0; i < superpage_bitmap_bits; i++)
		free_pages_in_superpage[i] = PAGES_PER_SUPERPAGE;

	/* that's it, now mark the memory used by the kernel (+ our arrays) as reserved */
	alloc_region(0, (size_t) kspace_end);
	printf("Memory allocator set up\n");
}

