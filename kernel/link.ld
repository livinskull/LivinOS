/* link.ld
 * Linker Script for LivinOS kernel
 */

ENTRY(start)
SECTIONS
{
	.mbHeader 0x100000: { /* code starts at 1MB */
	    mbHeader = .; _mbHeader = .; __mbHeader = .; 
	    *(.mbHeader)
	}

	.text :	
	{
		phys_kernel_start = .; _phys_kernel_start = .; __phys_kernel_start = .;
		*(.text)
		. = ALIGN(4096);
	}

	.data :
	{
		data = .; _data = .; __data = .;
		*(.data)
		*(.rodata)
		. = ALIGN(4096);
	}

	.bss :
	{
		bss = .; _bss = .; __bss = .;
		*(.bss)
		. = ALIGN(4096);
	}

	phys_kernel_end = .; _phys_kernel_end = .; __phys_kernel_end = .;
}

