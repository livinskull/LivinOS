#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

#include <stdint.h>

// defines for GRUB multiboot struct
#define MB_FLAG_MEMORY		1
#define MB_FLAG_DEVICE		2
#define MB_FLAG_CMDLINE		4
#define MB_FLAG_MODULES		8
#define MB_FLAG_SYM_AOUT	16
#define MB_FLAG_SYM_ELF		32
#define MB_FLAG_MMAP		64
#define MB_FLAG_DRIVES		128
#define MB_FLAG_CFGTABLE	256
#define MB_FLAG_BOOTLOADER_NAME 512
#define MB_FLAG_APM			1024

#define MB_MEMORY_AVAILABLE	1
#define MB_MEMORY_RESERVED  2


typedef struct {
	uint32_t tabsize;
	uint32_t strsize;
	uint32_t addr;
	uint32_t reserved;
}multiboot_aout_symbol_table;

typedef struct {
	uint32_t num;
	uint32_t size;
	uint32_t addr;
	uint32_t shndx;
}multiboot_elf_section_header_table;

typedef struct {
	uint32_t size;
	uint64_t addr;
	uint64_t len;
	uint32_t type;
}__attribute__((packed)) multiboot_mmap_entry;

typedef struct multiboot_module {
	uint32_t    mod_start;
	uint32_t    mod_end;
	char        *cmdline;
	uint32_t    reserved;
}__attribute__((packed)) multiboot_module;

// passed to kernel by GRUB
typedef struct {
	uint32_t flags;			// info wether other fields are present
	uint32_t mem_lower;		// memory below 1MB starting at 0x0
	uint32_t mem_upper;		// memory above 1MB
	uint32_t boot_device;	// device booted from
	uint32_t cmdline;		// address of cmdline string
	uint32_t mods_count;	// number of modules loaded
	uint32_t mods_addr;		// address of first module structure
	union {
         multiboot_aout_symbol_table aout_sym;
         multiboot_elf_section_header_table elf_sec;
    } u;
	uint32_t mmap_length;	// length of memory map
	uint32_t mmap_addr;		// address of memory map
	uint32_t drives_length; // total size of drive structures
	uint32_t drives_addr;   // address of drive structures
	uint32_t config_table;  // address of ROM configuration table
	uint32_t boot_loader_name; // address of the name of the bootloader
	uint32_t apm_table;		// address of APM table
	uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
   	uint16_t vbe_interface_seg;
   	uint16_t vbe_interface_off;
   	uint16_t vbe_interface_len;
}__attribute__ ((packed)) multiboot_info;

#endif
