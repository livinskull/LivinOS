#ifndef _GDT_H_
#define _GDT_H_

#include <stdint.h>


#define GDT_FLAG_DATASEG 0x02
#define GDT_FLAG_CODESEG 0x0A
#define GDT_FLAG_TSS     0x09
 
#define GDT_FLAG_SEGMENT 0x10
#define GDT_FLAG_RING0   0x00
#define GDT_FLAG_RING3   0x60
#define GDT_FLAG_PRESENT 0x80
 
#define GDT_FLAG_4K_GRAN 0x80
#define GDT_FLAG_32_BIT  0x40


typedef struct {
    uint16_t usLimit;
    uint16_t usBaseLow;
    uint8_t byBaseMiddle;
    uint8_t byAccess;
    uint8_t byGranularity;
    uint8_t byBaseHigh;
}__attribute__((packed)) GdtEntry;

typedef struct _gdt_ptr {
    uint16_t usLimit;
    uint32_t ulBase;
}__attribute__((packed)) GdtPtr;



void gdt_init(void);
void gdt_set_gate(uint32_t ulNum, uint32_t ulBase, uint32_t ulLimit, uint8_t byAccess, uint8_t byGran);


#endif
