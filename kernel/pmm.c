#include <pmm.h>
#include <stdlib.h>
#include <textmode.h>
#include <paging.h>


static uint8_t aBitmap[PMM_BITMAP_LENGTH];
static uint32_t iMemTotal = 32*1024*1024;	// 32MB default, if detection fails
static uint32_t iBitmapLen = (32*1024*1024/PMM_BLOCK_SIZE)/8;	// holds actual bitmap length in BYTES (adapted to available RAM)


// internal functions
static uint8_t pmm_getBit(uint32_t iBitNum);
static void pmm_unsetBit(uint32_t iBitNum);
static void pmm_setBit(uint32_t iBitNum);

// used == bit set


static inline void __attribute__((always_inline)) pmm_markRangeUnused(uint32_t iStart, uint32_t iEnd) {
	while (iStart < iEnd) {
		pmm_unsetBit(iStart / PMM_BLOCK_SIZE);
		iStart += PMM_BLOCK_SIZE;
	}
}

static inline void __attribute__((always_inline)) pmm_markRangeUsed(uint32_t iStart, uint32_t iEnd) {
	while (iStart < iEnd) {
		pmm_setBit(iStart / PMM_BLOCK_SIZE);
		iStart += PMM_BLOCK_SIZE;
	}
}

/**
 * Initializes pmm
 * @param m_info pointer to Multiboot-info structure
 */
void pmm_init(multiboot_info *m_info) {
	multiboot_mmap_entry *mmap;
	uint32_t i = 0;
	
	// init bitmap to all mem free
	for (i=0; i<PMM_BITMAP_LENGTH; ++i)
		aBitmap[i] = 0x00;
	
    
    
    
	if (m_info->flags & MB_FLAG_MEMORY) {
		iMemTotal = m_info->mem_lower + m_info->mem_upper;
		iBitmapLen = ((iMemTotal*1024)/PMM_BLOCK_SIZE)/8;
		kprintf("RAM: %u MB\nbitmaplen: %u\n", iMemTotal/1024, iBitmapLen);
	}
	
	// if we have a memory map, use it 
	if (m_info->flags & MB_FLAG_MMAP) {
		// mark all reserved memory blocks as used
		kprintf("used mem:\n");
		for (mmap = (multiboot_mmap_entry *) (m_info->mmap_addr + KERNEL_VIRTUAL_BASE); (uint32_t) mmap < m_info->mmap_addr + m_info->mmap_length; ++mmap) {
			if (mmap->type == MB_MEMORY_RESERVED) {	// if memory not free
				pmm_markRangeUsed((uint32_t) mmap->addr, (uint32_t) mmap->addr + (uint32_t) mmap->len);
                kprintf("0x%x - 0x%x\n", (uint32_t) mmap->addr, (uint32_t) mmap->addr + (uint32_t) mmap->len);
            }
		}
	}
	
	// dont use any memory below 1 MB (0x0 == NULL pointer, Video RAM)
	//pmm_markRangeUsed(0x0, 0x100000);
	

	// mark kernel memory as used
	pmm_markRangeUsed(0, ((uint32_t) &phys_kernel_end) - KERNEL_VIRTUAL_BASE);    
    
#if 0
	
	// mark multiboot-info as used
	if ((uint32_t) m_info > 0x100000) {
		pmm_setBit(((uint32_t) m_info)/PMM_BLOCK_SIZE);
		
		// mark module space as used, if any
		if (m_info->flags & MB_FLAG_MODULES) {
			multiboot_module *modules = (multiboot_module *) m_info->mods_addr;
			uint32_t addr;
			
			pmm_setBit(((uint32_t) modules)/PMM_BLOCK_SIZE);
			
			for (i=0; i < m_info->mods_count; ++i) {
				addr = modules[i].mod_start;
				while (addr < modules[i].mod_end) {
					pmm_setBit(addr/PMM_BLOCK_SIZE);
					addr += 0x1000;
				}
			}
		}
	}
#endif
}

/**
 * Searchs free memblock, marks as used and returns address
 * @return void* Pointer to memblock
 */
void *pmm_alloc() {
	uint32_t i,j;
	
	// start after 16MB (DMA)
	for (i = PMM_NUM_DMA_ELEMENTS/8; i<iBitmapLen; ++i) {
		if (aBitmap[i] != 0xFF) { // search only when not all 8 are used
			for (j=0; j<8; ++j) {
				if (pmm_getBit(i*8+j) == 0) {	// if block unused
                    //kprintf("free pmm block %d ", i*8+j);
					pmm_setBit(i*8+j);
                    //kprintf("now %d\n",pmm_getBit(i*8+j));
					return (void *)((i*8+j)*PMM_BLOCK_SIZE);
				}
			}
		}
	}
	
	return NULL;
}

/**
 * Searchs n free memblocks, marks them as used and returns address of first
 * @param uint32_t num bumber of pages to get
 * @return void* Pointer to memblock
 * @TODO: optimize (check at byte-level)
 */
void *pmm_range_alloc(uint32_t num) {
	uint32_t i, found = 0;
	
	// start after 16MB (DMA)
	for (i = PMM_NUM_DMA_ELEMENTS; i<iBitmapLen*8; ++i) {
		if (pmm_getBit(i) == 0) {
			++found;
			if (found == num) {
				pmm_markRangeUsed((i - found + 1)*PMM_BLOCK_SIZE, i*PMM_BLOCK_SIZE);
				return (void *) ((i-found + 1)*PMM_BLOCK_SIZE);
			}
		} else {
			found = 0;
		}		
	}
	
	return NULL;
}

/**
 * frees a previously alloced chunk of memoryblocks
 * @param void *p pointer to alloced memory
 * @param uint32_t num number of alloced blocks
 */
void pmm_range_free(void *p, uint32_t num) {
	// NULL pointer cant be free'd
	if (p == NULL) return;
	
	pmm_markRangeUnused((uint32_t) p, (uint32_t) p + num*PMM_BLOCK_SIZE);
}



/**
 * Searchs free memblock suitable for DMA, marks as used and returns address
 * @return void* Pointer to memblock
 */
void *pmm_alloc_dma() {
	uint32_t i,j;
	
	// start after 16MB (DMA)
	for (i = 0; i<PMM_NUM_DMA_ELEMENTS/8; ++i) {
		if (aBitmap[i] != 0xFF) { // search only when not all 8 are used
			for (j=0; j<8; ++j) {
				if (pmm_getBit(i*8+j) == 0) {	// if block unused
					pmm_setBit(i*8+j);
					return (void *)((i*8+j)*PMM_BLOCK_SIZE);
				}
			}
		}
	}
	
	return NULL;
}

/**
 * frees a previously alloced chunk of memory
 * @param void *p pointer to alloced memory
 */
void pmm_free(void *p) {
	uint32_t bitnum;
	
	// NULL pointer cant be free'd
	if (p == NULL) return;
	
	bitnum = ((uint32_t) p) / PMM_BLOCK_SIZE;
	if (pmm_getBit(bitnum))		// only free if its actually used
		pmm_unsetBit(bitnum);
}

/**
 * Sets bit in bitmap for block iBitNum
 * @param uint32_t iBitNum Bit to set
 */
void pmm_setBit(uint32_t iBitNum) {
	uint8_t b = (1 << (iBitNum % 8));
	aBitmap[iBitNum >> 3] |= b;
}

/**
 * Unsets bit in bitmap for iBitNum
 * @param uint32_t iBitNum Bit to set
 */
void pmm_unsetBit(uint32_t iBitNum) {
	uint8_t b = (1 << (iBitNum % 8));
	aBitmap[iBitNum >> 3] &= (~b);
}

/**
 * Gets bit from bitmap for iBitNum
 * @param uint32_t iBitNum Bit to get
 */
uint8_t pmm_getBit(uint32_t iBitNum) {
	uint8_t b = (1 << (iBitNum % 8));
    
	return (aBitmap[iBitNum >> 3] & b) ? 1 : 0;
}

