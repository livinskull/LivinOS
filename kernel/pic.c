#include <pic.h>
#include <io.h>


void pic_init(void) {
	/* init master PIC */
	outb(PIC_CMD_PORT_MASTER, PIC_ICW1_INIT | PIC_ICW1_SEND_ICW4);
	outb(PIC_DATA_PORT_MASTER, 32);	/* int number for irq0 => 32; first free after exceptions */
	outb(PIC_DATA_PORT_MASTER, 0x04); /* standard ICW3 */
	outb(PIC_DATA_PORT_MASTER, 0x01); /* 8086 operation mode */
	
	/* init slave PIC */
	outb(PIC_CMD_PORT_SLAVE, PIC_ICW1_INIT | PIC_ICW1_SEND_ICW4);
	outb(PIC_DATA_PORT_SLAVE, 40);	/* int number for irq8 */
	outb(PIC_DATA_PORT_SLAVE, 0x02); /* standard ICW3 */
	outb(PIC_DATA_PORT_SLAVE, 0x01); /* 8086 operation mode */
	
	/* set mask to 0 => enable all irqs */
	outb(PIC_DATA_PORT_MASTER, 0x00);
	outb(PIC_DATA_PORT_SLAVE, 0x00);
}


void pic_eoi(uint8_t byIrqNo) {
	if (byIrqNo > 7)
		outb(PIC_CMD_PORT_SLAVE, 0x20); // also send to slave
	
	outb(PIC_CMD_PORT_MASTER, 0x20);
}
