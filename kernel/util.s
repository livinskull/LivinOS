[global halt]
[global gdt_flush]
[global idt_load]
[global read_cr2]

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


;uint32_t read_cr2(void);
read_cr2:
	mov eax, cr2
	ret


readtsc:
	lfence		; serialize stuff
	rdtsc		; 64bit return is in edx:ax, rdtsc also uses edx:eax
	ret


