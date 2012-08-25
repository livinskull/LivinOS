#include <serial.h>
#include <textmode.h>
#include <io.h>


static uint16_t usBaseAddresses[4] = {0};
static uint8_t byNumPorts = 0;


void serial_init() {
	int i=0;
	// read base ports of COM ports
	// BDA is identity mapped already nothing todo
	
	// read num of COM ports from equipment word (xxx0 0000 0000)
	byNumPorts = (uint8_t) (((*((uint16_t *) 0x0410)) & 0xe00) >> 9);
	
	kprintf("found %d serial ports\n", byNumPorts);
	
	// com port address in BDA at 0x400
	usBaseAddresses[0] = *((uint16_t *) 0x0400);
	usBaseAddresses[1] = *((uint16_t *) 0x0402);
	usBaseAddresses[2] = *((uint16_t *) 0x0404);
	usBaseAddresses[3] = *((uint16_t *) 0x0406);
	
	for (i=0; i<byNumPorts; ++i)
		kprintf("  COM%d @ 0x%x\n", i, usBaseAddresses[i]);
}


void serial_init_communication(uint8_t byPortNum, uint32_t ulBaud, uint8_t byParity, uint8_t byBits) {
	uint16_t usDivisor;
	
	if (byPortNum >= byNumPorts || ulBaud == 0)
		return;
	
	usDivisor = 115200 / ulBaud;
	
	// deactivate ints
    outb(usBaseAddresses[byPortNum] + SERIAL_REG_INT_ENABLE, 0x00);
	
	// set DLAB to write divisor to 0 and 1
	outb(usBaseAddresses[byPortNum] + SERIAL_REG_LINE_CTRL, 0x80);
	
	// write divisor
	outb(usBaseAddresses[byPortNum] + 0, (uint8_t) (usDivisor & 0xff));
	outb(usBaseAddresses[byPortNum] + 1, (uint8_t) (usDivisor >> 8));
	
	// set bit count & parity & stuff + erset DLAB
	outb(usBaseAddresses[byPortNum] + SERIAL_REG_LINE_CTRL, ((byParity & 0x7) << 3) 
				| ((byBits-5) & 0x3));
	
	// 11000111, FIFO enable, clear recv & trans FIFO, recv FIFO trigger lvl
	outb(usBaseAddresses[byPortNum] + SERIAL_REG_FIFO_CTRL, 0xc7);
	
	// 1011 DTR high, RTS high, OUT1 low, OUT2 high (int output)
	outb(usBaseAddresses[byPortNum] + SERIAL_REG_MODEM_CTRL, 0x0b);
}


void serial_write_byte(uint16_t byPortNum, uint8_t chr) {
	if (byPortNum >= byNumPorts)
		return;
	
	while ((inb(usBaseAddresses[byPortNum] + SERIAL_REG_LINE_STATE) & 0x20) == 0);
	
	outb(usBaseAddresses[byPortNum], chr);
}


void serial_write(uint16_t byPortNum, const char *s) {
	if (!s)
		return;
	
	while (*s)
		serial_write_byte(byPortNum, *s++);
}

