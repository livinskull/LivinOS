[global halt]
[global gdt_flush]
[global idt_load]
[global read_cr0]
[global read_cr2]
[global read_cr3]
[global write_cr0]
[global write_cr3]
[global tlb_flush]

[extern gp]
[extern g_pIdt]


; halt forever
halt:
	cli
.loop:
	hlt
	jmp .loop
	ret


; reload GDT
gdt_flush:
        lgdt [gp]
        mov ax,0x10
        mov ds,ax
        mov es,ax
        mov fs,ax
        mov gs,ax
        mov ss,ax
        jmp 0x08:.flush2
.flush2:
        ret


;void idt_load(void);
idt_load:
	lidt [g_pIdt]
	ret


;uint32_t read_cr0(void);
read_cr0:
	mov eax, cr0
	ret

;uint32_t read_cr2(void);
read_cr2:
	mov eax, cr2
	ret

;uint32_t read_cr3(void);
read_cr3:
	mov eax, cr4
	ret

;void write_cr0(uint32_t val);
write_cr0:
	mov eax, [esp+4]
	mov cr0, eax
	ret

;void write_cr3(uint32_t val);
write_cr3:
	mov eax, [esp+4]
	mov cr3, eax
	ret



readtsc:
	lfence		; serialize stuff
	rdtsc		; 64bit return is in edx:ax, rdtsc also uses edx:eax
	ret


