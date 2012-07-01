#include <paging.h>
#include <util.h>
#include <pmm.h>
#include <string.h>
#include <textmode.h>

extern uint32_t kernel_stack;


void vmm_init() {
	uint32_t ulAddr,i,npt;
	uint32_t *pd = pmm_alloc();
	uint32_t *pt;
	
	/* dont use paging functions here as paging isnt enabled yet */
	
	
	/* identity map all kernel mem including alloced mem */
	/* kernel_stack comes after kernel_end; add some buffer (1MB) for pmm_allocs so far 
	 (they _should_ alloc before kernel anyway, but who cares) */
	npt = 1+ ((kernel_stack & 0xfffff000) + 0x100000) / PAGE_SIZE / PAGE_TABLE_LENGTH;
	kprintf("num pagetables for kernel: %d\n", npt);
	pt = pmm_range_alloc(npt);
	
	if (pd == 0 || pt == 0) {
		puts("allocate space for page stuff failed\n");
		return;
	}
	
	memset(pd, 0, PAGE_SIZE);
	memset(pt, 0, npt * PAGE_SIZE);
	
	
	for (ulAddr = i = 0; i < npt*PAGE_TABLE_LENGTH; ++i, ulAddr += 4096)
		pt[i] = ulAddr | PTE_WRITE_ACCESS | PTE_PRESENT;
	
	/* enter tables to pd */
	for (i=0; i<npt; ++i)
		pd[i] = ((uint32_t) pt + 4096*i) | PDE_WRITE_ACCESS | PDE_PRESENT;
	
	/* map pd as last pt in kernel space */
	pd[VMM_PD_ADDRESS >> 22] = (uint32_t) pd | PDE_WRITE_ACCESS | PDE_PRESENT;
	
	puts("Enabling paging...\n");
	/* enable paging */
	write_cr3((uint32_t) pd);
	write_cr0(read_cr0() | 0x80000000);
}


void *vmm_get_physaddr(void *virtualaddr) {
    uint32_t pdindex = (uint32_t)virtualaddr >> 22;
    uint32_t ptindex = (uint32_t)virtualaddr >> 12 & 0x03FF; 
    uint32_t *pd = (uint32_t *) VMM_PD_ADDRESS;
	uint32_t *pt = ((uint32_t *)VMM_PT_ADDRESS) + (0x400 * pdindex);
    
	if ((pd[pdindex] & PDE_PRESENT) == 0)
		return 0;
    
	if ((pt[ptindex] & PTE_PRESENT) == 0)
		return 0;
 
    return (void *)((pt[ptindex] & ~0xFFF) + ((uint32_t)virtualaddr & 0xFFF));
}


void map_page(uint32_t physaddr, uint32_t virtualaddr, uint32_t ulFlags) {
    uint32_t pdindex = virtualaddr >> 22;
    uint32_t ptindex = virtualaddr >> 12 & 0x03FF;
    uint32_t *pd = (uint32_t *)VMM_PD_ADDRESS;
	uint32_t *pt = 0;

	// Make sure that both addresses are page-aligned.
	if (virtualaddr & 0xfff || physaddr & 0xfff) {
		puts("ERROR: trying to page unaligned memory\n");
		return;
	}
	
	
	if ((pd[pdindex] & PDE_PRESENT) == 0) {
		/* create new pt */
		pd[pdindex] = (uint32_t) pmm_alloc() | ulFlags;
		__asm__ __volatile__("invlpg %0" : : "m" (* (char*) (pdindex << 22)));
		memset((void *) (pt[ptindex] & PAGE_ADDRESS_MASK), 0, PAGE_SIZE);
	}
 
	if (pt == 0)
		return;
 
    pt = ((uint32_t *)VMM_PT_ADDRESS) + (0x400 * pdindex);
    
	if (pt[ptindex] & PTE_PRESENT) {
		kprintf("ERROR: virt 0x%x already mapped\n", virtualaddr);
		return;
	}
	
	pt[ptindex] = ((uint32_t)physaddr) | (ulFlags & 0xFFF) | 0x01; // Present
 
    __asm__ __volatile__("invlpg %0" : : "m" (* (char*) virtualaddr));
}


void *vmm_alloc(uint8_t bKernel) {
	uint32_t *pd = (uint32_t *)VMM_PD_ADDRESS;
	uint32_t pdindex = 0, ptindex = 0;
	uint32_t *pt;
	
	
	for (pdindex = bKernel ? 0 : (VMM_USERSPACE_START >> 22); pdindex < (bKernel ? (VMM_USERSPACE_START >> 22) : PAGE_DIRECTORY_LENGTH); ++pdindex) {
		if (pd[pdindex] & PDE_PRESENT) {
			pt = ((uint32_t *)VMM_PT_ADDRESS) + (0x400 * pdindex);
			for (ptindex = pdindex == 0 ? 1 : 0; ptindex < PAGE_TABLE_LENGTH; ++ptindex) {
				if ((pt[ptindex] & PTE_PRESENT) == 0) {
					void *pNew = pmm_alloc();
					if (!pNew)
						return (void *) 0;
					/* alloced mem is NOT initialized to 0 */
					pt[ptindex] = (uint32_t) pNew | PTE_WRITE_ACCESS | PTE_PRESENT;
					if (!bKernel)
						pt[ptindex] |= PTE_USERLEVEL;
					//tlb_flush((pdindex << 22) | (ptindex << 12));
					__asm__ __volatile__("invlpg %0" : : "m" (* (char*) ((pdindex << 22) | (ptindex << 12))));
					return (void *) ((pdindex << 22) | (ptindex << 12));
				}
			}
		}
	}
	
	// create new pt
	for (pdindex = bKernel ? 0 : (VMM_USERSPACE_START >> 22); pdindex < (bKernel ? (VMM_USERSPACE_START >> 22) : PAGE_DIRECTORY_LENGTH); ++pdindex) {
		/* find and empty pd-entry and add a new pt there */
		if ((pd[pdindex] & PDE_PRESENT) == 0) {
			void *pNew = pmm_alloc();
			if (!pNew)
				return (void *) 0;
			
			pt = ((uint32_t *)VMM_PT_ADDRESS) + (0x400 * pdindex);
			
			pd[pdindex] = (uint32_t) pNew | PDE_WRITE_ACCESS | PDE_PRESENT;
			if (!bKernel)
				pd[pdindex] |= PDE_USERLEVEL;

			__asm__ __volatile__("invlpg %0" : : "m" (* (char*) pt));
			
			memset((void *) pt, 0, PAGE_SIZE);
			
			pNew = pmm_alloc();
			if (!pNew)
				return (void *) 0;
			/* alloced mem is NOT initialized to 0 */
			ptindex = pdindex == 0 ? 1 : 0;
			pt[ptindex] = (uint32_t) pNew | PTE_PRESENT | PTE_WRITE_ACCESS;
			if (!bKernel)
				pt[ptindex] |= PTE_USERLEVEL;

			__asm__ __volatile__("invlpg %0" : : "m" (* (char*) ((pdindex << 22) | (ptindex << 12))));

			return (void *) ((pdindex << 22)  | (ptindex << 12));
		}
	}
	
	
	return (void *) 0;
}


void vmm_free(void *pVirtualAddr) {
	uint32_t pdindex = (uint32_t) pVirtualAddr >> 22;
    uint32_t ptindex = (uint32_t) pVirtualAddr >> 12 & 0x03FF;
    //uint32_t *pd = (uint32_t *)VMM_PD_ADDRESS;
	uint32_t *pt = ((uint32_t *)VMM_PT_ADDRESS) + (0x400 * pdindex);;
	
	pmm_free(pt[ptindex] & PAGE_ADDRESS_MASK);
	pt[ptindex] = 0;
}

