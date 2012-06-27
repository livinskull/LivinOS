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

#include "config.h"




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
	
	
	// enable hw ints
	__asm__ __volatile__ ("sti");
	
	
	puts("init done!\n");

	
	while (1)
		__asm__ __volatile__("hlt");
	//halt();
}


