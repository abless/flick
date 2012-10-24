/* 
 * mm/vmm.c
 * Copyright (C) 2003 Alexander Blessing
 *
 * handles the virtual memory stuff
 */

#include <stdio.h>
#include <memory.h>
#include <kernel/multiboot.h>

#define PTE_SIZE 4
#define PDE_SIZE 4

#define PG_DIR_START 0xFFFFF000	/* because of */
#define PG_TABLE_START 0xFFC00000 /* the selfmapping trick */

#define ADDR_TO_PDE(virt) (PG_DIR_START + (virt >> 22) * PDE_SIZE)
#define ADDR_TO_PTE(virt) (PG_TABLE_START + (virt >> 12) * PTE_SIZE)

extern addr_t *pdir;
extern addr_t kernel_end;
extern addr_t kspace_end;

addr_t kernel_heap;
unsigned long pmem_size = 0;

/* map physical to virtual address */
int mm_map(addr_t virt, addr_t phys, size_t num_pages)
{
	addr_t *pde, *pte;
	int i;
	
	/* round up to a multiple of PAGE SIZE */
	virt = PAGE_ALIGN(virt);
	phys = PAGE_ALIGN(phys);

	for (i = 0; i < num_pages; 
			i++, virt += PAGE_SIZE, phys += PAGE_SIZE) {
		pde = (addr_t *) ADDR_TO_PDE(virt);
		if ((*pde & IS_PRESENT) == 0) {	/* non-present PDE */
			addr_t *pt;
			pt = phys_alloc();
			if (pt == NULL)
				panic("out of memory\n");
			*pde = (addr_t) pt | PRIV_KERNEL | PRIV_RW | PRIV_PRES;
		}
		pte = (addr_t *) ADDR_TO_PTE(virt);
		*pte = (addr_t) phys | PRIV_KERNEL | PRIV_RW | PRIV_PRES;
		invlpg(virt);
	}
	return 0;
}	

/* unmap virtual address */
int mm_unmap(addr_t virt, size_t num_pages)
{
	int i;
	
	virt = PAGE_ALIGN(virt);
	for (i = 0; i < num_pages; i++, virt += PAGE_SIZE) {
		*((addr_t*) ADDR_TO_PTE(virt)) = 0;
		invlpg(virt);
	}
	return 0;
}

/* get more memory from the heap */
void *morecore(size_t n)
{
	addr_t new_heap, start, *phys;
	
	n = PAGE_ALIGN(n);	/* make n be a multiple of 4 kb */
	
	start = kernel_heap;
	new_heap = kernel_heap + n;
	
	for (; kernel_heap < new_heap; kernel_heap += PAGE_SIZE) {
		phys = phys_alloc();
		if (phys == NULL)
			return (char*) -1;	/* out of memory */
		if (mm_map(kernel_heap, (addr_t) phys, 1) != 0)
			return NULL;
	}
	return (void*) start;	/* return start address */
}		

void init_map(void)
{
	addr_t *pt;
	unsigned int num_ptables;
	unsigned long addr = 0;
	int n = 0, i = 0;

	/* map kernel region to ensure that EIP will be mapped
	 * when paging is enabled
	 */
	num_ptables = (unsigned int) (PAGE_ALIGN((addr_t) &kspace_end) / 0x400000) + 1;
	while(num_ptables-- > 0) {
		pt = phys_alloc();
		if (pt == NULL)
			panic("out of memory\n");
		for (i = 0; i < 1024; i++) {
			pt[i] = addr | PRIV_KERNEL | PRIV_RW | PRIV_PRES;
			addr += PAGE_SIZE;
		}
		if (n != 1023)	/* don't overwrite selfmapping entry */
			pdir[n++] = (addr_t) pt | PRIV_KERNEL | PRIV_RW | PRIV_PRES;
	
	}
}

/* intialize the virtual memory manager */
void init_vmm(void)
{
	/* initialize virtual memory
	 * this is how we will map everything:
	 *
	 * virtual address 0 -> physical address 0 
	 * the heap will be set up right after the end of kernel space
	 */

	load_pdir(pdir);
	enable_paging();
	
	kernel_heap = kspace_end + 1;
	printf("Paging: "); log("enabled\n", GREEN);
}

/* initialize the whole memory manager */
void init_mm(void)
{
	if (mboot_info->flags & MB_INFO_MEMORY)
		pmem_size = 0x100000 + (mboot_info->mem_upper * 0x400);
	else
		panic("unable to determine physical memory size");
	
	printf("Physical memory size (KB): %i\n", (pmem_size / 1024));
	init_paging();	/* initialize paging */
	init_phys_alloc();	/* initialize physical memory allocator */
	init_map();	/* map kernel region to ensure EIP is mapped when enabling paging */
	
	init_vmm();	/* initialize virtual memory manager and enable paging */
}
