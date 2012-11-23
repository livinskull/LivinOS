#ifndef _PAGING_H_
#define _PAGING_H_

#include <stdint.h>


#define PAGE_DIRECTORY_LENGTH	1024
#define PAGE_TABLE_LENGTH		1024
#define PAGE_SIZE				4096
#define PAGE_ADDRESS_MASK 		0xFFFFF000


// pagetable entries
#define PTE_PRESENT			1	// if set, page is currently in memory
#define PTE_WRITE_ACCESS	2	// if set, write access is allowed, else readonly
#define PTE_USERLEVEL	  	4	// if bit is set, everybody can access the page
#define PTE_WRITETHROUGH	8	// if set writethrough caching, else write-back caching
#define PTE_NO_CACHING		16	//
#define PTE_ACCESSED		32
#define PTE_DIRTY			64	// set if page has been written to
#define PTE_PAT				128
#define PTE_GLOBAL_PAGE		256
#define PTE_AVAILABLE1		512
#define PTE_AVAILABLE2		1024
#define PTE_AVAILABLE3		2048

// pagedirectory entries
#define PDE_PRESENT			1
#define PDE_WRITE_ACCESS	2	
#define PDE_USERLEVEL	  	4	// if bit is set, everybody can access the page
#define PDE_WRITETHROUGH	8
#define PDE_NO_CACHING		16
#define PDE_ACCESSED		32
#define PDE_4MB_PAGES		128 // is set 4M pagesize else 4kb
#define PDE_GLOBAL_PAGE		256 //ignored 
#define PDE_AVAILABLE1		512
#define PDE_AVAILABLE2		1024
#define PDE_AVAILABLE3		2048


// TODO fix this shit, page stuff is at end of 4 gig, kernel space at start...
//#define VMM_USERSPACE_START	0x40000000		// 1GB mark
#define KERNEL_VIRTUAL_BASE     0xC0000000      // 3GB userspace, 1GB kernel


#define VMM_PD_ADDRESS      0xFFFFF000  // the pd is mapped as a pagetable to that address
#define VMM_PT_ADDRESS      0xFFC00000


void vmm_init();
void vmm_switch_pagedirectory(uint32_t ulPhysPd);
void *vmm_alloc(uint8_t bKernel);
void map_page(uint32_t physaddr, uint32_t virtualaddr, uint32_t ulFlags);
void *vmm_get_physaddr(void *virtualaddr);
void vmm_free(void *pVirtualAddr);
void *vmm_automap(void *pPhysical, uint8_t bKernel);


#endif
