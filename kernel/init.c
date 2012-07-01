#include <stdint.h>
#include <gdt.h>
#include <idt.h>
#include <inthandler.h>
#include <multiboot.h>
#include <textmode.h>
#include <util.h>
#include <pmm.h>
#include <pit.h>
#include <pic.h>
#include <string.h>
#include <paging.h>

#include "config.h"


void memtest() {
	void *a = 0, *b = 0;
	
	a = vmm_alloc(1);
	kprintf("a = vmm_alloc: 0x%x\n", a);
	b = vmm_alloc(1);
	kprintf("b = vmm_alloc: 0x%x\n", b);
	
	vmm_free(a);
	puts("vmm_free(a)\n");
	
	a = vmm_alloc(1);
	kprintf("a = vmm_alloc: 0x%x\n", a);
	
	vmm_free(a);
	puts("vmm_free(a)\n");
	vmm_free(b);
	puts("vmm_free(b)\n");
	
}


void init(multiboot_info *pMBoot) {
	textmode_clearscreen(' ', 0);
	
	kprintf("Setting up GDT...\n");
	gdt_init();
	
	kprintf("Setting up IDT...\n");
	idt_init();
	int_init();
	
	/* TODO check if APIC there and use; also use apic timer then */
	kprintf("Programming PIC...\n");
	pic_init();
	
	kprintf("Setting system frequency to %uHz...\n", CONFIG_SYSTEM_FREQUENCY);
	pit_phase(CONFIG_SYSTEM_FREQUENCY);
	
	pmm_init(pMBoot);
	
	vmm_init();
	
	
	// enable hw ints
	__asm__ __volatile__ ("sti");
	
	
	puts("init done!\n");
	
	
	memtest();
	

	
	while (1)
		__asm__ __volatile__("hlt");
	//halt();
}


