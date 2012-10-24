; boot/start.asm
; Copyright (C) 2002 Alexander Blessing
;
; called by bootloader (hopefully GRUB)
; jumps to main kernel after having set up some low-level stuff

[BITS 32]	; want 32 bit code
[global start]	; entry point
[extern kmain]
[extern disable_nmi]

section .text

start:
	cli	; disable all interrupts
	call disable_nmi
	jmp over_data	; don't read the data stuff

	; multiboot header for GRUB 
	MULTIBOOT_PAGE_ALIGN   equ 1<<0
	MULTIBOOT_MEMORY_INFO  equ 1<<1 
	MULTIBOOT_HEADER_MAGIC equ 0x1BADB002

	MULTIBOOT_HEADER_FLAGS equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO 
	CHECKSUM               equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS) 
	
	; build Multiboot header
	align 4
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd CHECKSUM

over_data:
	; check if we are called by GRUB
	mov [mboot_info], ebx	; store multiboot info
	cmp eax, 0x2BADB002
	sete [mboot_valid]

	; load our own GDT
	; GRUB sets up one only temporarily
	lgdt [gdt_ptr]	; now load the GDT...
	mov ax,DS_SELECTOR	; ...and update segment registers
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov fs,ax
	mov gs,ax
	jmp CS_SELECTOR:go_on	; do a JMP to initialize CS register

go_on:
	; zero BSS
	extern bss, kernel_end	
	mov edi,bss
	mov ecx,kernel_end
	sub ecx,edi
	xor eax,eax
	rep stosb 

	mov esp,stack_top	; and set up our stack

	; now, let's set up an IDT
	; up to now, we have 256 descriptors
	; so first, fill every descriptor with
	; a common interrupt handler routine
	; afterwards, we'll change this
	mov ecx,(idt_end - idt) >> 3	; number of descriptors
	mov edi,idt	; idt location
	mov esi,common_isr	; common handler to esi

fill_idt:
	mov eax,esi
	mov [edi],ax	; low offset
	shr eax,16
	mov [edi+6],ax	; high offset
	add edi,8	; descriptor length
	loop fill_idt

	lidt [idt_ptr]	; ok, load the IDT

	; that's it - do a jmp to the main C function
	jmp kmain

common_isr:
	; this is the common interrupt service routine
	; this means, we haven't implemented this
	; interrupt yet
	; so we just return
	iretd

section .data	; the data section

[global mboot_valid]
mboot_valid:
	dd 0	; for verification of a valid Multiboot header
[global mboot_info]
mboot_info:
	dd 0	; holds information about the Multiboot header
	
; Global Descriptor Table
gdt:
; NULL descriptor
	dw 0
	dw 0
	db 0
	db 0
	db 0
	db 0

[global DS_SELECTOR]
DS_SELECTOR	equ $ - gdt
	dw 0FFFFh
	dw 0
	db 0
	db 92h	; present, ring 0, data, expand-up, writable
	db 0CFh	; page-granular (4 gig limit), 32-bit
	db 0

[global CS_SELECTOR]
CS_SELECTOR	equ $ - gdt
	dw 0FFFFh
	dw 0
	db 0
	db 9Ah	; present,ring 0,code,non-conforming,readable
	db 0CFh	; page-granular (4 gig limit), 32-bit
	db 0

[global USER_DATA_SEL]
USER_DATA_SEL	equ $ - gdt
	dw 0FFFFh
	dw 0
	db 0
	db 0F2h
	db 0CFh
	db 0

[global USER_CODE_SEL]
USER_CODE_SEL	equ $ - gdt
	dw 0FFFFh
	dw 0
	db 0
	db 0FAh
	db 0CFh
	db 0
	
gdt_end: 
gdt_ptr:
	dw gdt_end - gdt - 1
	dd gdt

; Interrupt Descriptor Table
; have 256 descriptor here, all having
; the same interrupt handler
; will change this at runtime
[global idt]
idt:
%rep 256
	dw 0	; offset 15:0
	dw CS_SELECTOR	; selector
	dw 0x8E00	; present, ring 0, '386 interrupt gate
	dw 0	; offset 31:16
%endrep
idt_end: 

idt_ptr:
	dw idt_end - idt - 1; IDT limit
	dd idt	; start of IDT

section .bss
	resb 8192	; 8 kilobytes for stack
	stack_top:	; top of our stack here
