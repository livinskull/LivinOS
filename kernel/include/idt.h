#ifndef _IDT_H_
#define _IDT_H_

#include <stdint.h>


#define IDT_NUM_ENTRIES		256

/* flags */
#define IDT_FLAG_INTERRUPT_GATE	0x0e
#define IDT_FLAG_PRESENT		0x80
#define IDT_FLAG_RING0			0x00
#define IDT_FLAG_RING3			0x60


typedef struct {
    uint16_t usBaseLow;
    uint16_t usSel;
    uint8_t always0;
    uint8_t byFlags;
    uint16_t usBaseHigh;
} __attribute__((packed)) IdtEntry;

typedef struct {
    uint16_t usLimit;
    uint32_t ulBase;
} __attribute__((packed)) IdtPtr;



void idt_init(void);
void idt_set_gate(uint8_t byNum, uint32_t ulBase, uint16_t usSel, uint8_t byFlags);



#endif
