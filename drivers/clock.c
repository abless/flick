/*
 * drivers/clock.c
 * Copyright (C) 2003 Alexander Blessing
 *
 * forces task switchs
 */

#include <stdio.h>
#include <kernel/interrupt.h>
#include <kernel/process.h>

#define QUANTUM 5 

extern process_t *current;
extern int sched_enable;

volatile unsigned long ticks = 0;
volatile int quantum = QUANTUM;

void init_clock(void)
{
	/* reprogram clock interrupt to 100 Hz */
	out(0x43, 0x36);
	out(0x40, (11392 & 0xFF));
	out(0x40, ((11392 >> 8) & 0xFF));

	/* enable clock interrupt */
	put_handler(0x20, (void*) &INT_IRQ0, GATE_DEFAULT);
	enable_irq(0);
}

void *clock(void *current_esp)
{
	void *sp = current_esp;

	ticks++;
	if (--quantum == 0) {
		quantum = QUANTUM;
		if (sched_enable) {
			if (current != NULL && (current->run == 1))	/* if current process is valid */
				current->stack = (void*) ((int) current_esp);
			sp = schedule();	/* pick another process */
		}
	}
	return sp;
}
