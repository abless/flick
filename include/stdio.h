/*
 * include/stdio.h
 * Copyright (C) 2002 Alexander Blessing
 */

#ifndef _INCLUDE_STDIO_H_
#define _INCLUDE_STDIO_H_

#include <kernel/console.h>
#include <stdarg.h>
#include <types.h>

#define printf printk
#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

#define MAXLEN 1024

/* video attributes */
#define INTENSITY 0x08

#define WHITE 0x07
#define RED 0x04
#define GREEN 0x02
#define CYAN 0x03 
#define YELLOW 0x06
#define BLUE 0x01
#define MAGENTA 0x05

#define in(port) ({ \
		unsigned char v; \
		__asm__ __volatile__ ("inb %%dx, %%al" \
				: "=a" (v) : "d" (port)); \
		v; \
})

#define out(port, value) \
		__asm__ __volatile__ ("outb %%al, %%dx" :: "a" (value), \
					"d" (port))

/* prototypes */

void clear(void);
void init_screen(void);

void flush(console_t *);

int debug(const char *, ...);
int log(char *, int);
inline int putc(int);
int printc(int, const char *, ...);
int printk(const char *, ...);
void panic(const char *, ...);
void vsprintf(char *, const char *, va_list);

void lock_mutex(semaphore *);
void unlock_mutex(semaphore *);

#endif
