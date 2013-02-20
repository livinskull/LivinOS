#include <paging.h>
#include <util.h>
#include <pmm.h>
#include <string.h>
#include <textmode.h>

extern uint32_t BootPageDirectory;

// used for paging kernel when pmm not initialized yet
static uint32_t s_aFirstPt[PAGE_TABLE_LENGTH];
static uint8_t s_bFirst = 1;



void vmm_init() {
    uint32_t *pd = (uint32_t *) &BootPageDirectory;
    
	memset(s_aFirstPt, 0, PAGE_SIZE);

	/* map pd as last pt in kernel space */
	pd[VMM_PT_ADDRESS >> 22] = (uint32_t) ((uint32_t) pd-KERNEL_VIRTUAL_BASE) | PDE_WRITE_ACCESS | PDE_PRESENT;
}


void *vmm_get_physaddr(void *virtualaddr) {
    uint32_t pdindex = (uint32_t) virtualaddr >> 22;
    uint32_t ptindex = (uint32_t) virtualaddr >> 12 & 0x03FF; 
    uint32_t *pd = (uint32_t *) VMM_PD_ADDRESS;
	uint32_t *pt = ((uint32_t *)VMM_PT_ADDRESS) + (0x400 * pdindex);
    
	if ((pd[pdindex] & PDE_PRESENT) == 0)
		return 0;
    
	if ((pt[ptindex] & PTE_PRESENT) == 0)
		return 0;
 
    return (void *)((pt[ptindex] & PAGE_ADDRESS_MASK) + ((uint32_t) virtualaddr & 0xFFF));
}

#if 0
void map_page(uint32_t physaddr, uint32_t virtualaddr, uint32_t ulFlags) {
    uint32_t pdindex = virtualaddr >> 22;
    uint32_t ptindex = virtualaddr >> 12 & 0x03FF;
    uint32_t *pd = (uint32_t *) VMM_PD_ADDRESS;
	uint32_t *pt = ((uint32_t *) VMM_PT_ADDRESS) + (0x400 * pdindex);

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
 
    //pt = ((uint32_t *)VMM_PT_ADDRESS) + (0x400 * pdindex);
    
	if (pt[ptindex] & PTE_PRESENT) {
		kprintf("ERROR: virt 0x%x already mapped\n", virtualaddr);
		return;
	}
	
	pt[ptindex] = ((uint32_t) physaddr) | (ulFlags & 0xFFF) | 0x01; // Present
 
    __asm__ __volatile__("invlpg %0" : : "m" (* (char*) virtualaddr));
}
#endif


void *vmm_alloc(uint8_t bKernel) {
	void *pNew = pmm_alloc();
	
	if (!pNew) {
        kprintf("OUT OF RAM!\n");
		return (void *) 0;
    }
	
	return vmm_automap(pNew, bKernel);
}

void *vmm_automap(void *pPhysical, uint8_t bKernel) {
	uint32_t *pd = (uint32_t *) VMM_PD_ADDRESS;
	uint32_t pdindex = 0, ptindex = 0, offset=0;
	uint32_t *pt;
    
    // auto-align address and add offset on return 
    offset = (uint32_t) pPhysical & 0xfff;
    pPhysical = (uint32_t) pPhysical & 0xfffff000;

	
    if (bKernel)  {
        for (pdindex = (KERNEL_VIRTUAL_BASE >> 22); pdindex < PAGE_DIRECTORY_LENGTH; ++pdindex) {
            if ((pd[pdindex] & PDE_PRESENT) && ((pd[pdindex] & PDE_4MB_PAGES) == 0)) {
                pt = ((uint32_t *) VMM_PT_ADDRESS) + (0x400 * pdindex);
                
                for (ptindex = 0; ptindex < PAGE_TABLE_LENGTH; ++ptindex) {
                    if ((pt[ptindex] & PTE_PRESENT) == 0) {
                        pt[ptindex] = (uint32_t) pPhysical | PTE_WRITE_ACCESS | PTE_PRESENT;
                        __asm__ __volatile__("invlpg %0" : : "m" (* (char*) ((pdindex << 22) | (ptindex << 12))));
                        return ((void *) ((pdindex << 22) | (ptindex << 12))) + offset;
                    }
                }
            }
        }
        
        // create new pt
        for (pdindex = (KERNEL_VIRTUAL_BASE >> 22); pdindex < PAGE_DIRECTORY_LENGTH; ++pdindex) {
            /* find and empty pd-entry and add a new pt there */
            if (((pd[pdindex] & PDE_PRESENT) == 0) && ((pd[pdindex] & PDE_4MB_PAGES) == 0)) {
                void *pNew = s_bFirst?s_aFirstPt-KERNEL_VIRTUAL_BASE:pmm_alloc();
                if (!pNew)
                    return (void *) 0;
                
                pt = ((uint32_t *)VMM_PT_ADDRESS) + (0x400 * pdindex);
                
                pd[pdindex] = (uint32_t) pNew | PDE_WRITE_ACCESS | PDE_PRESENT;
                __asm__ __volatile__("invlpg %0" : : "m" (* (char*) pt));
               
                memset((void *) pt, 0, PAGE_SIZE);
                
                ptindex = pdindex == 0 ? 1 : 0;
                pt[ptindex] = (uint32_t) pPhysical | PTE_PRESENT | PTE_WRITE_ACCESS;

                __asm__ __volatile__("invlpg %0" : : "m" (* (char*) ((pdindex << 22) | (ptindex << 12))));
                return ((void *) ((pdindex << 22)  | (ptindex << 12))) + offset;
            }
        }
    }
	
	return (void *) 0;
}


void vmm_free(void *pVirtualAddr) {
	uint32_t pdindex = (uint32_t) pVirtualAddr >> 22;
    uint32_t ptindex = (uint32_t) pVirtualAddr >> 12 & 0x03FF;
	uint32_t *pt = ((uint32_t *) VMM_PT_ADDRESS) + (0x400 * pdindex);;
	
	// free physical page
	pmm_free((void *) (pt[ptindex] & PAGE_ADDRESS_MASK));
	pt[ptindex] = 0;
    __asm__ __volatile__("invlpg %0" : : "m" (* (char*) pVirtualAddr));
}

