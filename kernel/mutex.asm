; kernel/mutex.asm
; Copyright (C) 2002 Alexander Blessing
;
; handles mutex functions
; mutex are very important - they are needed when
; entering region where only *one* process should do the work
; at a moment

[BITS 32]

[global lock_mutex]
lock_mutex:
	push eax;
	push edx;
	push ebp;
	mov ebp,esp
	mov edx,[ss:ebp+16]	; MOV the parameter of the function to edx
	mov al,1
.loop:
	xchg [edx],al
	cmp al,1
	je .loop
	pop ebp;
	pop edx;
	pop eax;
	ret

[global unlock_mutex]
unlock_mutex:
	push eax;
	push edx;
	push ebp;
	mov ebp,esp
	mov edx,[ss:ebp+16]
	mov byte [edx],0
	pop ebp;
	pop edx;
	pop eax;
	ret
