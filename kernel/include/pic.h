#ifndef _PIC_H_
#define _PIC_H_

#include <stdint.h>


#define PIC_CMD_PORT_MASTER		0x20
#define PIC_CMD_PORT_SLAVE		0xA0
#define PIC_DATA_PORT_MASTER	0x21
#define PIC_DATA_PORT_SLAVE		0xA1


#define PIC_ICW1_SEND_ICW4		0x01
#define PIC_ICW1_SINGLE_MODE	0x02
#define PIC_ICW1_INIT			0x10



void pic_init(void);
void pic_eoi(uint8_t byIrqNo);

#endif
