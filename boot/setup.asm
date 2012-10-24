; boot/setup.asm
; Copyright (C) 2002 Alexander Blessing
;
; sets up the ISRs for various interrupts

[BITS 32]	; protected mode
[global put_handler]	; called by an extern function
[extern idt]	; the IDT table

; this functions modifies an interrupt descriptor
put_handler:
	push ebx	; save
	push edx	; registers
	push eax	; that will 
	push ebp	; be changed

	mov ebp, esp
	mov ebx, [ss:ebp + 20]	; interrupt number
	mov eax, [ss:ebp + 24]	; interrupt handler
	mov edx, idt	; idt location

	; change address of handler
	shl ebx, 3	; need to multiply by 8
	mov [edx + ebx], ax	; low offset
	shr eax, 16
	mov [edx + ebx + 6], ax	; high offset

	; change flags
	mov ax, [ss:ebp + 28]	; gate flags
	mov [edx + ebx + 4], ax
	
	pop ebp	; restore 
	pop eax	; registers 
	pop edx	; that were 
	pop ebx	; changed	
	
	ret	; return to caller
