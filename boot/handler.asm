; boot/handler.asm
; Copyright (C) 2002 Alexander Blessing
;
; contains the interrupt handlers (Interrupt Service Routines)

[BITS 32]	; protected mode
[extern DS_SELECTOR]
[extern fault]

; macros for saving and restoring registers 
%macro SAVE 0
	; save some registers
	pushad
	push ds
	push es
	push fs
	push gs
	
	mov eax, DS_SELECTOR 
	mov ds, eax
	mov es, eax
	mov fs, eax
	mov gs, eax
%endmacro

%macro RESTORE 0
	; restore saved registers
	pop gs
	pop fs
	pop es
	pop ds
	popad
%endmacro

%macro PUSH_REGS 1
	SAVE
	mov eax, %1
	push eax
%endmacro

%macro POP_REGS 0
	pop eax
	RESTORE
%endmacro

; ISR00 (Divide Error)
[global ISR00]
ISR00:
	PUSH_REGS 0
	call fault

; ISR01 (Debug Exception)
[global ISR01]
ISR01:
	PUSH_REGS 1
	call fault
	
; ISR03 (Breakpoint)
[global ISR03]
ISR03:
	PUSH_REGS 3
	call fault

; ISR04 (Overflow)
[global ISR04]
ISR04:
	PUSH_REGS 4
	call fault

; ISR05 (Bounds Check)
[global ISR05]
ISR05:
	PUSH_REGS 5
	call fault

; ISR06 (Invalid Opcode)
[global ISR06]
ISR06:
	PUSH_REGS 6
	call fault

; ISR07 (Coprocessor Not Available)
[global ISR07]
ISR07:
	PUSH_REGS 7
	call fault

; ISR08 (Double Fault)
[global ISR08]
ISR08:
	PUSH_REGS 8
	call fault

; ISR09 (Coprocessor Segment Overrun)
[global ISR09]
ISR09:
	PUSH_REGS 9
	call fault

; ISR10 (Invalid TSS)
[global ISR10]
ISR10:
	PUSH_REGS 10
	call fault

; ISR11 (Segment Not Present)
[global ISR11]
ISR11:
	PUSH_REGS 11
	call fault

; ISR12 (Stack Exception)
[global ISR12]
ISR12:
	PUSH_REGS 12
	call fault

; ISR13 (Global Protection Fault)
[global ISR13]
ISR13:
	PUSH_REGS 13
	call fault
	
; ISR14 (Page Fault)
[global ISR14]
[extern page_fault]
ISR14:
	PUSH_REGS 14	

	call page_fault

	POP_REGS
	iretd

; ISR16 (Coprocessor Error)
[global ISR16]
ISR16:
	PUSH_REGS 16
	call fault

; IRQ0 (clock)
[global INT_IRQ0]
[extern clock]
INT_IRQ0:
	SAVE
	
	push esp	; push current esp
	call clock
	add esp, 4
	mov esp, eax	; get new esp 

	; send EOI to master PIC
	mov al, 0x20
	out 0x20, al

	RESTORE
	iretd	; cause a task switch if necessary

; IRQ1 (keyboard) 
[global INT_IRQ1]
[extern kbd_handler]
INT_IRQ1:
	SAVE ; save registers
	
	call kbd_handler	; call the handler
	
	; send EOI to master PIC
	mov al, 0x20 
	out 0x20, al 
	
	RESTORE	; restore registers
	iretd
