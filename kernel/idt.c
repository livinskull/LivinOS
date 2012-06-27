#include <idt.h>
#include <util.h>
#include <string.h>


/* Intel manuals: The base addresses of the IDT should be aligned on an 8-byte 
 * boundary to maximize performance of cache line fills. */
static IdtEntry s_Idt[IDT_NUM_ENTRIES] __attribute__ ((aligned (8)));
IdtPtr g_pIdt;


void idt_set_gate(uint8_t byNum, uint32_t ulBase, uint16_t usSel, uint8_t byFlags) {
	s_Idt[byNum].usBaseLow 	= (uint16_t) (ulBase & 0xFFFF);
	s_Idt[byNum].usBaseHigh = (uint16_t) (ulBase >> 16);
	s_Idt[byNum].usSel 		= usSel;
	s_Idt[byNum].byFlags 	= byFlags;
	s_Idt[byNum].always0 	= 0;
}


void idt_init(void) {
	g_pIdt.usLimit = sizeof(IdtEntry) * IDT_NUM_ENTRIES - 1;
	g_pIdt.ulBase = (uint32_t) s_Idt;
	
	
	memset((void *) &s_Idt, 0, sizeof(IdtEntry) * IDT_NUM_ENTRIES);
	
	idt_load();
}


