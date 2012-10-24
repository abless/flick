/*
 * kernel/main.c
 * Copyright (C) 2003 Alexander Blessing
 *
 * start of kernel 
 */

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <driver/clock.h>
#include <driver/keyboard.h>
#include <kernel/interrupt.h>
#include <kernel/kernel.h>
#include <kernel/process.h>

#define FLICK "Flick/i386"

void init_interrupts(void)
{
	init_pics(0x20, 0x28);
	
	/* set up interrupt handlers */
	init_kbd();
	init_clock();
	
	put_handler(0, (void*) &ISR00, GATE_DEFAULT);	/* Divide Error */
	put_handler(1, (void*) &ISR01, GATE_DEFAULT);	/* Debug Exception */
	put_handler(3, (void*) &ISR03, GATE_DEFAULT);	/* Breakpoint */
	put_handler(4, (void*) &ISR04, GATE_DEFAULT);	/* Overflow */
	put_handler(5, (void*) &ISR05, GATE_DEFAULT);	/* Bounds Check */
	put_handler(6, (void*) &ISR06, GATE_DEFAULT);	/* Invalid Opcode */
	put_handler(7, (void*) &ISR07, GATE_DEFAULT);	/* Coprocessor not available */
	put_handler(8, (void*) &ISR08, GATE_DEFAULT);	/* Double Fault */
	put_handler(9, (void*) &ISR09, GATE_DEFAULT);	/* Coprocessor Segment Overrun */
	put_handler(10, (void*) &ISR10, GATE_DEFAULT);	/* Invalid TSS */
	put_handler(11, (void*) &ISR11, GATE_DEFAULT);	/* Segment not present */
	put_handler(12, (void*) &ISR12, GATE_DEFAULT);	/* Stack exception */
	put_handler(13, (void*) &ISR13, GATE_DEFAULT);	/* GPF handler */
	put_handler(14, (void*) &ISR14, GATE_DEFAULT);	/* Page fault */
	put_handler(16, (void*) &ISR16, GATE_DEFAULT);	/* Coprocessor Error */
	
	enable_nmi();
	sti();	/* enable interrupts */

	printf("%s initializing:\n\n", FLICK);
}

void idle(void)
{
	/* this is the idle process
	 * it is chosen when no other processes are waiting for the CPU */
	while (1);
}

void kmain(void)
{
	init_screen();
	init_interrupts();
	init_mm();
	init_proc();
	
	create_process((int) idle, SCHED_LOW, PROC_KERNEL);
	set_sched(SCHED_ENABLE);	/* finally enable scheduler */
	idle();
}
