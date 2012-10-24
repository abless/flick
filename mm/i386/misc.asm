; mm/i386/misc.asm
; Copyright (C) 2003 Alexander Blessing
; 
; contains several low level functions for memory management

[BITS 32]

[global readCR3]
readCR3:
	mov eax, cr3
	ret

[global readCR2]
readCR2:
	mov eax, cr2
	ret

[global load_pdir]
load_pdir:
	push ebp
	mov ebp, esp
	mov eax, [ebp + 8]
	mov cr3, eax
	pop ebp
	ret

[global enable_paging]
enable_paging:
	mov eax, cr0
	or eax, 0x80000000
	mov cr0, eax
	jmp .end
.end:
	ret
