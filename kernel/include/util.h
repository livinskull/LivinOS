#ifndef _UTIL_H_
#define _UTIL_H_

#include <stdint.h>



extern void halt(void);
extern void gdt_flush(void);
extern void idt_load(void);

extern uint32_t read_cr0(void);
extern uint32_t read_cr2(void);
extern uint32_t read_cr3(void);
extern void write_cr0(uint32_t val);
extern void write_cr3(uint32_t val);


uint32_t sprintf(char *buff, const char *format,...);


#endif

