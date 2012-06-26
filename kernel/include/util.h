#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>


extern void halt(void);
extern void gdt_flush(void);
extern void idt_load(void);

extern uint32_t read_cr2(void);


#endif

