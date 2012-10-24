/*
 * include/kernel/kernel.h
 * Copyright (C) 2002 Alexander Blessing
 *
 * contains protoypes for kernel functions
 */

#ifndef _INCLUDE_KERNEL_KERNEL_H_
#define _INCLUDE_KERNEL_KERNEL_H_

#define lock cli
#define unlock sti

/* prototypes */
void enable_nmi(void);
void disable_nmi(void);

void cli(void);
void sti(void);
void hlt(void);
void reboot(void);
void halt(void);

#endif
