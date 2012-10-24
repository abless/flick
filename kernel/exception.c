/*
 * kernel/exception.c
 * Copyright (C) 2003 Alexander Blessing
 */

#include <stdio.h>
#include <kernel/kernel.h>
#include <kernel/console.h>

extern console_t *curcons;

typedef struct {
	int num;
	int gs;
	int fs;
	int es;
	int ds;
	int edi;
	int esi;
	int ebp;
	int esp;
	int ebx;
	int edx;
	int ecx;
	int eax;
	int error;
	int eip;
	int cs;
	int eflags;
} fault_t;

char *ex_types[] = {"Divide Error", "Debug Exception", "", "Breakpoint",
		    "Overflow", "Bounds check", "Invalid opcode",
		    "Coprocessor not available", "Double fault",
		    "Coprocessor segment overrun", "invalid TSS", 
		    "Segment not present", "Stack exception",
		    "General Protection Fault", "Page fault", " ", 
		    "Coprocessor error"};

void fault(fault_t args)
{
	debug("\nFatal exception:\n\n");

	debug("Exception #%i (%s)\n", args.num,  ex_types[args.num]);
	
	debug("EAX: 0x%x ECX: 0x%x EBX: 0x%x EDX: 0x%x\n", args.eax, args.ecx, args.ebx, args.edx);
	debug("EDI: 0x%x ESI: 0x%x EBP: 0x%x ESP: 0x%x\n", args.edi, args.esi, args.ebp, args.esp);
	debug("GS: 0x%x FS: 0x%x DS: 0x%x ES: 0x%x\n", args.gs, args.fs, args.ds, args.es);
	debug("EIP: 0x%x CS: 0x%x EFLAGS: 0x%x\n", args.eip, args.cs, args.eflags);
	debug("Error code: 0x%x\n", args.error);

	/* halt */
	cli(); hlt();
}
