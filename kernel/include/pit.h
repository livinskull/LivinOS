#ifndef _PIT_H_
#define _PIT_H_

#include <stdint.h>


// the default internal clock rate to calculate the divisor
#define PIT_CLOCK       1193180

// channel 0 == irq 0 == int0x20 == timer int
#define PIT_CHANNEL0_DATA_PORT  0x40
#define PIT_CHANNEL1_DATA_PORT  0x41
#define PIT_CHANNEL2_DATA_PORT  0x42
#define PIT_COMMAND_REGISTER    0x43


/* bits in the PIT_COMMAND_REGISTER */
#define PIT_BCD_MODE            1
#define PIT_OPERATING_MODE0 0   // interrupt on terminal count
#define PIT_OPERATING_MODE1 2   // hw re-triggerable one-shot
#define PIT_OPERATING_MODE2 4   // rate generator
#define PIT_OPERATING_MODE3 6   // square wave generator
#define PIT_OPERATING_MODE4 8   // sw triggered strobe
#define PIT_OPERATING_MODE5 10  // hw triggered strobe

#define PIT_ACCESS_MODE_LATCHCOUNT      0
#define PIT_ACCESS_MODE_LOBYTE          16
#define PIT_ACCESS_MODE_HIBYTE          32

#define PIT_SELECT_CHANNEL0             0
#define PIT_SELECT_CHANNEL1             64
#define PIT_SELECT_CHANNEL2             128



void pit_phase(uint32_t ulHz);

#endif
