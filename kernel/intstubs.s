[extern int_handler]



%macro gen_int_stub 1
[global int_stub_%1]
int_stub_%1:
	push 0		; no error code
	push %1		; int number
	jmp int_common_stub
%endmacro

%macro gen_int_stub_error 1
[global int_stub_%1]
int_stub_%1:
	push %1		; int number
	jmp int_common_stub
%endmacro


; exceptions
gen_int_stub 0
gen_int_stub 1
gen_int_stub 2
gen_int_stub 3
gen_int_stub 4
gen_int_stub 5
gen_int_stub 6
gen_int_stub 7
gen_int_stub_error 8
gen_int_stub 9
gen_int_stub_error 10
gen_int_stub_error 11
gen_int_stub_error 12
gen_int_stub_error 13
gen_int_stub_error 14
gen_int_stub 15
gen_int_stub 16
gen_int_stub_error 17
gen_int_stub 18
gen_int_stub 19
gen_int_stub 20
gen_int_stub 21
gen_int_stub 22
gen_int_stub 23
gen_int_stub 24
gen_int_stub 25
gen_int_stub 26
gen_int_stub 27
gen_int_stub 28
gen_int_stub 29
gen_int_stub 30
gen_int_stub 31

; IRQs (0-16 => PIC)
gen_int_stub 32
gen_int_stub 33
gen_int_stub 34
gen_int_stub 35
gen_int_stub 36
gen_int_stub 37
gen_int_stub 38
gen_int_stub 39
gen_int_stub 40
gen_int_stub 41
gen_int_stub 42
gen_int_stub 43
gen_int_stub 44
gen_int_stub 45
gen_int_stub 46
gen_int_stub 47






int_common_stub:
	pushad
	
	; save segment registers
	push ds
	push es
	push fs
	push gs

	cld			; reset direction flag to known state, EFLAGS for restoring pushed by CPU
	
	; load kernel segments
	mov ax, 0x10
	mov ds, ax

	push esp
	call int_handler
	mov esp, eax		; get new registers, this basically does multitasking

	; restore registers
	pop gs
	pop fs
	pop es
	pop ds
	popad
	
	add esp, 8		; delete error code and int number
	iretd
	