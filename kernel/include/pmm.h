#ifndef _PMM_H_
#define _PMM_H_

#include <stdint.h>
#include <multiboot.h>


#define PMM_MAX_MEM				1024ULL*1024ULL*1024ULL*4ULL		// 4GB
#define PMM_BLOCK_SIZE 			4096								// 4K = pagesize
#define PMM_NUM_DMA_ELEMENTS 	((16*1024*1024) / PMM_BLOCK_SIZE)	// resevre 16K lowmem for DMA transfers
#define PMM_BITMAP_LENGTH 		(PMM_MAX_MEM/PMM_BLOCK_SIZE)/8



/* defined by linker script */
extern uint32_t phys_kernel_end;
extern uint32_t phys_kernel_start;
extern uint32_t kernel_stack;


void pmm_init(multiboot_info *pInfo);
void *pmm_alloc();
void *pmm_alloc_dma();
void pmm_free(void *p);
void *pmm_range_alloc(uint32_t num);
void pmm_range_free(void *p, uint32_t num);



#endif
