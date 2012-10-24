/*
 * kernel/kernel.c
 * Copyright (C) 2002 Alexander Blessing
 * 
 * contains kernel functions
 */

#include <stdio.h>
#include <kernel/interrupt.h>

unsigned short int ocw1 = 0xFFFF;	/* short int = 16 bits */

/**********************
 * PIC operations here
 *********************/

/* init_pics()
 * init the PICs and remap them
 */
void init_pics(int pic1, int pic2)
{
	/* send ICW1 */
	out(PIC1, ICW1);
	out(PIC2, ICW1);

	/* send ICW2 */
	out(PIC1 + 1, pic1);	/* remap */
	out(PIC2 + 1, pic2);	/* pics */

	/* send ICW3 */
	out(PIC1 + 1, 4);	/* IRQ2 -> connection to slave */
	out(PIC2 + 1, 2);

	/* send ICW4 */
	out(PIC1 + 1, ICW4);
	out(PIC2 + 1, ICW4);

	/* disable all IRQs */
	out(PIC1 + 1, 0xFF);
	out(PIC2 + 1, 0xFF);
}

/* enable_irq()
 * sends command to PIC to enable an IRQ
 */
void enable_irq(int irq)
{
	ocw1 &= ~(1 << irq);	/* enable propriate bit with shifting to left
				   invert the thing to enable the interrupt
				   use AND operation to leave the other bits
				   as they are
				 */
	if (irq < 8)
		out(PIC1 + 1, ocw1&0xFF);	/* AND with 0xFF to clear the high 8 
					  	   bits because we send to PIC1
						 */
	else
		out(PIC2 + 1, ocw1 >> 8);	/* move high 8 bits to low 8 bits
						   since we send to PIC2
						 */
}

/* disable_irq()
 * sends a command to PIC to disable an IRQ
 */
void disable_irq(int irq)
{
	ocw1 |= (1 << irq);	/* shift left to disable the propriate bit
				   OR to not change the mask
				 */

	if (irq < 8)
		out(PIC1 + 1, ocw1&0xFF);	/* AND with 0xFF to clear the
						   high 8 bits since we send to PIC1
						 */
	else
		out(PIC2 + 1, ocw1 >> 8);	/* move high 8 bits to low 8 bits since
						   we send to PIC2
						 */
}

inline void write_nmi(unsigned char nmi)
{
	out(0x70, nmi);
	in(0x71);
}

inline void enable_nmi(void)
{
	write_nmi(ENABLE_NMI);
}

inline void disable_nmi(void)
{
	write_nmi(DISABLE_NMI);
}

/**********************
 * CPU operations here
 *********************/

/* sti() 
 * enable interrupts
 */
inline void sti(void)
{
	__asm__("sti");
}

/* cli()
 * disable interrupts
 */
inline void cli(void)
{
	__asm__("cli");
}

/* hlt()
 * halts the CPU
 */
inline void hlt(void)
{
	__asm__("hlt");
}

inline void save(void)
{
	/* don't have anything here yet */
}

/* halt()
 * halts the CPU 
 */
void halt(void)
{
	/* in fact, we do nothing else than saving
	 * everything that needs to be saved and 
	 * wait until s.o. shuts down the PC 
	 */
	
	cli();
	save();	/* save important things */

	debug("\nPower down");
	hlt();	/* wait for shutdown */
}

/* reboot()
 * reboots the CPU
 */
void reboot(void)
{
	unsigned char temp;

	cli();
	save();
	
	/* flush the keyboard controller */
	do
	{
		temp = in(0x64);
		if (temp & 1) {
			(void) in(0x60);
			continue;
		}
	}while(temp & 2);

	/* send the CPU reset line */
	out(0x64, 0xFE);
	
	hlt();
}
