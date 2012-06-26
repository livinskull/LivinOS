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
	
	kprintf("Programming PIC...\n");
	pic_init();
	
	kprintf("Setting system frequency to %uHz...\n", CONFIG_SYSTEM_FREQUENCY);
	pit_phase(CONFIG_SYSTEM_FREQUENCY);
	

	pmm_init(pMBoot);
	
	
	// TODO remap PIC/APIC and enable hw ints
	
	
	puts("init done!\n");

	
	halt();
}


