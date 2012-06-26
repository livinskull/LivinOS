#include <stdint.h>
#include <gdt.h>
#include <idt.h>
#include <inthandler.h>
#include <multiboot.h>
#include <textmode.h>
#include <util.h>
#include <pmm.h>


#include <string.h>




void init(multiboot_info *pMBoot) {
    int j=0;
 
	textmode_clearscreen(' ', 0);
	
	kprintf("Setting up GDT...\n");
	gdt_init();
	
	kprintf("Setting up IDT...\n");
	idt_init();
	int_init();

	pmm_init(pMBoot);
	
	
	// TODO remap PIC/APIC and enable hw ints
	
	
	puts("init done!\n");
	//j/=0;

	
	halt();
}


