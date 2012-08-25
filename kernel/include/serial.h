#ifndef _SERIAL_H_
#define _SERIAL_H_

#include <stdint.h>


/* serial registers */
#define SERIAL_REG_RECV			0
#define SERIAL_REG_TRANSMIT		0
#define SERIAL_REG_INT_ENABLE	1
#define SERIAL_REG_INT_ID		2
#define SERIAL_REG_FIFO_CTRL	2
#define SERIAL_REG_LINE_CTRL	3
#define SERIAL_REG_MODEM_CTRL	4
#define SERIAL_REG_LINE_STATE	5
#define SERIAL_REG_MODEM_STATE	6
#define SERIAL_REG_SCRATCH		7


/* bits for Int enable reg */
#define SERIAL_IER_DATAREADY		1	// int if data incoming 
#define SERIAL_IER_TRANSMIT_EMPTY	2	// int if transmit buffer empty
#define SERIAL_IER_STATUS			4	// int if transmission error?
#define SERIAL_IER_MODEM_STATUS		8	// int if delta bits set?



void serial_init();
void serial_init_communication(uint8_t byPortNum, uint32_t ulBaud, uint8_t byParity, uint8_t byBits);
void serial_write_byte(uint16_t byPortNum, uint8_t chr);
void serial_write(uint16_t byPortNum, const char *s);



#endif
