#include <pit.h>
#include <io.h>


void pit_phase(uint32_t ulHz) {
	int divisor = PIT_CLOCK / ulHz;
	
	outb(PIT_COMMAND_REGISTER, PIT_OPERATING_MODE3 | PIT_SELECT_CHANNEL0
			| PIT_ACCESS_MODE_LOBYTE | PIT_ACCESS_MODE_HIBYTE);
	outb(PIT_CHANNEL0_DATA_PORT, divisor & 0xff);	// low byte first
	outb(PIT_CHANNEL0_DATA_PORT, divisor >> 8);
}
