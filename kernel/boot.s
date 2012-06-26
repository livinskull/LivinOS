; boot.s - LivinOS boot code

[section .mbHeader]

MBOOT_PAGE_ALIGN    equ 1<<0    ; Load kernel and modules on a page boundary
MBOOT_MEM_INFO      equ 1<<1    ; Provide your kernel with memory info
MBOOT_SYM_ELF		equ 32
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Multiboot Magic value
MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]

[GLOBAL start]
[GLOBAL kernel_stack] 
[EXTERN init]
[GLOBAL mboot]
[EXTERN phys_kernel_start]
[EXTERN bss]
[EXTERN phys_kernel_end]

mboot:
  ; GRUB options
  dd  MBOOT_HEADER_MAGIC
  dd  MBOOT_HEADER_FLAGS 
  dd  MBOOT_CHECKSUM
   
  dd  mboot                     ; Location of this descriptor
  dd  phys_kernel_start         ; Start of kernel '.text' (code) section.
  dd  bss                       ; End of kernel '.data' section.
  dd  phys_kernel_end           ; End of kernel.
  dd  start                     ; Kernel entry point (initial EIP).


[section .text]

kernel_stack dd 0


start:
  mov eax, phys_kernel_end
  add eax, 0x10000
  mov [kernel_stack], eax
  mov esp, eax
  push ebx                   ; Load multiboot header location

  cli                         ; Disable interrupts.
  call init
  jmp $                       ;loop forever n case kernel ends unexpected

