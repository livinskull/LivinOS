#ifndef _INT_HANDLER_H_
#define _INT_HANDLER_H_

#include <stdint.h>


typedef struct {
	uint32_t gs, fs, es, ds;			/* pushed manually */
	uint32_t edi, esi, ebp, discard_esp, ebx, edx, ecx, eax;	/* pushed by pushad */
	uint32_t ulIntNo, ulErrCode;		/* pushed by int stubs */
	uint32_t eip, cs, eflags, esp, ss;	/* pushed automatically by CPU */
}__attribute__((packed)) CpuState;


typedef void (*IntHandler)(CpuState*);


void int_init();
CpuState *int_handler(CpuState *pState);
int int_register_handler(uint8_t byNum, IntHandler pHandler);


/* exceptions */
extern void int_stub_0();
extern void int_stub_1();
extern void int_stub_2();
extern void int_stub_3();
extern void int_stub_4();
extern void int_stub_5();
extern void int_stub_6();
extern void int_stub_7();
extern void int_stub_8();
extern void int_stub_9();
extern void int_stub_10();
extern void int_stub_11();
extern void int_stub_12();
extern void int_stub_13();
extern void int_stub_14();
extern void int_stub_15();
extern void int_stub_16();
extern void int_stub_17();
extern void int_stub_18();
extern void int_stub_19();
extern void int_stub_20();
extern void int_stub_21();
extern void int_stub_22();
extern void int_stub_23();
extern void int_stub_24();
extern void int_stub_25();
extern void int_stub_26();
extern void int_stub_27();
extern void int_stub_28();
extern void int_stub_29();
extern void int_stub_30();
extern void int_stub_31();

/* irqs */
extern void int_stub_32();
extern void int_stub_33();
extern void int_stub_34();
extern void int_stub_35();
extern void int_stub_36();
extern void int_stub_37();
extern void int_stub_38();
extern void int_stub_39();
extern void int_stub_40();
extern void int_stub_41();
extern void int_stub_42();
extern void int_stub_43();
extern void int_stub_44();
extern void int_stub_45();
extern void int_stub_46();
extern void int_stub_47();



#endif
