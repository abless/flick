/*
 * include/kernel/interrupt.h
 * Copyright (C) 2002 Alexander Blessing
 *
 * contains definitions for interrupt handling (including IRQ)
 */

#ifndef _INCLUDE_KERNEL_INTERRUPT_H_
#define _INCLUDE_KERNEL_INTERRUPT_H_

#define GATE_DEFAULT 0x8E00

/* definitions for PIC operations */
#define PIC1 0x20
#define PIC2 0xA0
#define ICW1 0x11
#define ICW4 0x01

#define ENABLE_NMI 0x00
#define DISABLE_NMI 0x80

/* prototypes */
void put_handler(unsigned int, void *, unsigned short int);
void init_pics(int, int);
void enable_irq(int);
void disable_irq(int);
void send_eoi(int);

/* ISR prototypes */
void ISR00(void);
void ISR01(void);
void ISR03(void);
void ISR04(void);
void ISR05(void);
void ISR06(void);
void ISR07(void);
void ISR08(void);
void ISR09(void);
void ISR10(void);
void ISR11(void);
void ISR12(void);
void ISR13(void);
void ISR14(void);
void ISR16(void);
void INT_IRQ0(void);
void INT_IRQ1(void);

#endif
