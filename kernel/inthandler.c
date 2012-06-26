#include <inthandler.h>
#include <idt.h>
#include <io.h>
#include <util.h>
#include <textmode.h>


/* installs default handlers */
void int_init() {
	/* exceptions */
	idt_set_gate(0, (uint32_t) int_stub_0, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(1, (uint32_t) int_stub_1, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(2, (uint32_t) int_stub_2, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(3, (uint32_t) int_stub_3, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(4, (uint32_t) int_stub_4, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(5, (uint32_t) int_stub_5, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(6, (uint32_t) int_stub_6, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(7, (uint32_t) int_stub_7, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(8, (uint32_t) int_stub_8, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(9, (uint32_t) int_stub_9, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(10, (uint32_t) int_stub_10, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(11, (uint32_t) int_stub_11, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(12, (uint32_t) int_stub_12, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(13, (uint32_t) int_stub_13, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(14, (uint32_t) int_stub_14, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(15, (uint32_t) int_stub_15, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(16, (uint32_t) int_stub_16, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(17, (uint32_t) int_stub_17, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(18, (uint32_t) int_stub_18, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(19, (uint32_t) int_stub_19, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(20, (uint32_t) int_stub_20, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(21, (uint32_t) int_stub_21, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(22, (uint32_t) int_stub_22, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(23, (uint32_t) int_stub_23, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(24, (uint32_t) int_stub_24, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(25, (uint32_t) int_stub_25, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(26, (uint32_t) int_stub_26, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(27, (uint32_t) int_stub_27, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(28, (uint32_t) int_stub_28, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(29, (uint32_t) int_stub_29, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(30, (uint32_t) int_stub_30, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	idt_set_gate(31, (uint32_t) int_stub_31, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_RING0 | IDT_FLAG_PRESENT);
	
	
	/* TODO IRQs */
	
	
	/* TODO syscall sw int(s) */
}



/* exception handler */
void int_exception_handler(CpuState *r) {
	static const char *pExceptions[] = {
		"Division by zero",
		"Debug",
		"Non Maskable Interrupt",
		"Breakpoint",
		"Into Detected Overflow",
		"Out of Bounds",
		"Invalid Opcode",
		"No Coprocessor available",

		"Double Fault",
		"Coprocessor Segment Overrun",
		"Bad TSS",
		"Segment Not Present",
		"Stack Fault",
		"General Protection Fault",
		"Page Fault",
		"Unknown Interrupt",

		"Coprocessor Fault",
		"Alignment Check",
		"Machine Check",
		"SIMD Floating Point",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",

		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved"
	};
	
	// TODO make it BLUE! (or maybe sth nicer, users hate bluescreens :D)
	textmode_clearscreen(' ',0);
	
	kprintf("Exception, system halted: %s (%u)\n", pExceptions[r->ulIntNo], r->ulIntNo);
	kprintf("error code: 0x%x\n", r->ulErrCode);
	
	/* full register dump */
	kprintf("EAX = %x\t\tEBX = %x\t\tECX = %x\n",r->eax, r->ebx, r->ecx);
	kprintf("EDX = %x\t\tESP = %x\t\tEBP = %x\n",r->edx, r->esp, r->ebp);
	kprintf("ESI = %x\t\tEDI = %x\t\tEIP = %x\n",r->esi, r->edi, r->eip);
	kprintf("\nES = %x\t\tCS = %x\t\tSS = %x\n",r->es, r->cs, r->ss);
	kprintf("DS = %x\t\tFS = %x\t\tGS = %x\n", r->ds, r->fs, r->gs);
	kprintf("\nFlags:\t\tStack Trace\n");
	kprintf("CF %d\t\t%x: %x\n", r->eflags & 1, (uint32_t) (r->esp), *(uint32_t *) (r->esp));
	kprintf("PF %d\t\t%x: %x\n", (r->eflags & 4)/4, (uint32_t) (r->esp + 4), *(uint32_t *) (r->esp + 4));
	kprintf("AF %d\t\t%x: %x\n", (r->eflags & 16)/16, (uint32_t) (r->esp + 8), *(uint32_t *) (r->esp + 8));
	kprintf("ZF %d\t\t%x: %x\n", (r->eflags & 64)/64, (uint32_t) (r->esp + 12), *(uint32_t *) (r->esp + 12));
	kprintf("SF %d\t\t%x: %x\n", (r->eflags & 128)/128, (uint32_t) (r->esp + 16), *(uint32_t *) (r->esp + 16));
	kprintf("DF %d\t\t%x: %x\n", (r->eflags & 1024)/1024, (uint32_t) (r->esp + 20), *(uint32_t *) (r->esp + 20));
	kprintf("OF %d\t\t%x: %x\n", (r->eflags & 2048)/2048, (uint32_t) (r->esp + 24), *(uint32_t *) (r->esp + 24)); 

	if (r->ulIntNo == 14) {		//if page-fault
		kprintf("Page adress: %x\n", read_cr2());
		puts(r->ulErrCode & 1 ? "General Protection Fault " : "Page not present ");
		puts(r->ulErrCode & 2 ? "when WRITING " : "when READING ");
		puts(r->ulErrCode & 4 ? "in Userspace\n" : "in Kernelspace\n");
	}
	
	halt();
}


CpuState *int_handler(CpuState *pState) {
	CpuState *pNew = pState;	/* return old state if not scheduler int */
	
	
	if (pState->ulIntNo < 32) {
		/* EXCEPTION! */
		/* panic screen here and some exception handling in the future */
		int_exception_handler(pState);
	} else if (pState->ulIntNo == 0x20) {
		/* IRQ0, timer int, call scheduler */
		//__asm__ __volatile__("nop");
	} else {
		/* other stuff, hw IRQs or soft int, call handler if registered, else do nothing */
		
		/* hw IRQs need EOI signalled */
		/* TODO move this stuff to a separate PIC file; use apic if exists */
		if (pState->ulIntNo > 40)
			outb(0xA0, 0x20);	// send pic eoi to slave
		
		// send pic eoi to master
		outb(0x20, 0x20);
	}
	
	return pNew;
}
