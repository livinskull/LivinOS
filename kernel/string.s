[global memset]
[global memcpy]


; void memset(void *pDest, uint8_t byValue, uint32_t ulNum)
memset:
	push ebp
	mov ebp, esp
	push edi
	
	xchg bx,bx

	mov edi, [ebp + 8]
	mov eax, [ebp + 12]
	mov edx, [ebp + 16]

	cmp edx, 4
	jb .byteset

	mov ecx, edx
	shr ecx, 2		; ecx = count / 4

	rep stosd		; set stuff in dwords

.byteset:
	mov ecx, edx
	and ecx, 11b	; set remaining <4 bytes
	jz .end
	
	rep stosb

.end:
	pop edi
	pop ebp
	ret


; void memcpy(void *pDest, const void *pSrc, uint32_t ulNum)
memcpy:
	push ebp
	mov ebp, esp
	push edi
	push esi

	xchg bx,bx
	
	mov edi, [ebp + 8]
	mov esi, [ebp + 12]
	mov edx, [ebp + 16]

	cmp edx, 4
	jb .bytecpy

	mov ecx, edx
	shr ecx, 2		; ecx = count / 4

	rep movsd		; set stuff in dwords

.bytecpy:
	mov ecx, edx
	and ecx, 11b	; set remaining <4 bytes
	jz .end
	
	rep movsb

.end:
	pop esi
	pop edi
	pop ebp
	ret

