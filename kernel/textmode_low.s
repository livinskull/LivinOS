[extern g_Pos]

[global textmode_clearscreen]


; implemented in asm for greater performance
; void textmode_clearscreen(uint8_t byChar, uint8_t byCol)
; direction flag should be cleared by cdecl convention, eax, edx & ecx are allowed to be trashed
textmode_clearscreen:
	push ebp
	mov ebp, esp

	push edi

	mov dh, [ebp + 12]
	mov dl, [ebp + 8]
	mov ax, dx
	shl eax, 16
	mov ax, dx

	mov edi, 0xb8000

	mov ecx, 1000	; 80x25 = 4000 bytes = 1000 dwords
	rep stosd

	mov DWORD [g_Pos], 0

	pop edi

	pop ebp
	ret

