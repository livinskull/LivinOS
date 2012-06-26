#include <gdt.h>
#include <util.h>


#define GDT_NUM_ENTRIES		5


static GdtEntry gdt[GDT_NUM_ENTRIES];
GdtPtr gp;


void gdt_init(void) {
	gp.usLimit = sizeof(GdtEntry) * GDT_NUM_ENTRIES - 1;
	gp.ulBase = (uint32_t) &gdt;


	// NULL descriptor
	gdt_set_gate(0, 0, 0, 0, 0);

	// kernel code segment
	gdt_set_gate(1, 0, 0xFFFFFFFF, GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT, GDT_FLAG_32_BIT | GDT_FLAG_4K_GRAN);

	// kernel data segment
	gdt_set_gate(2, 0, 0xFFFFFFFF, GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT, GDT_FLAG_32_BIT | GDT_FLAG_4K_GRAN);

	//user segments
	gdt_set_gate(3, 0, 0xFFFFFFFF, GDT_FLAG_SEGMENT | GDT_FLAG_CODESEG | GDT_FLAG_PRESENT | GDT_FLAG_RING3, GDT_FLAG_32_BIT | GDT_FLAG_4K_GRAN);
	gdt_set_gate(4, 0, 0xFFFFFFFF, GDT_FLAG_SEGMENT | GDT_FLAG_DATASEG | GDT_FLAG_PRESENT | GDT_FLAG_RING3, GDT_FLAG_32_BIT | GDT_FLAG_4K_GRAN);

	//TSS
	//memset(&tss, 0x00, 32);
	//tss[2] = 0x10; 	//kernel stack segment
	//gdt_set_gate(5, (uint32_t) tss, sizeof(tss), GDT_FLAG_TSS | GDT_FLAG_PRESENT | GDT_FLAG_RING3, 0);


	gdt_flush();
	// reload task register
	//__asm__ __volatile__("ltr %%ax" : : "a" (5 << 3));
}


void gdt_set_gate(uint32_t ulNum, uint32_t ulBase, uint32_t ulLimit, uint8_t byAccess, uint8_t byGran) {
	gdt[ulNum].usBaseLow = (uint16_t) (ulBase & 0xFFFF);
	gdt[ulNum].byBaseMiddle = (uint8_t) ((ulBase >> 16) & 0xFF);
	gdt[ulNum].byBaseHigh = (uint8_t) ((ulBase >> 24) & 0xFF);
	gdt[ulNum].usLimit = (uint16_t) (ulLimit & 0xFFFF);
	gdt[ulNum].byGranularity = (uint8_t) ((ulLimit >> 16) & 0x0F);
	gdt[ulNum].byGranularity |= (byGran & 0xF0);
	gdt[ulNum].byAccess = byAccess;
}

