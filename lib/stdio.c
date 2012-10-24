/*
 * lib/stdio.h
 * Copyright (C) 2002 Alexander Blessing
 * 
 * contains several self-written function for standard I/O
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>	/* for va_start & Co. */
#include <kernel/interrupt.h>
#include <kernel/kernel.h>
#include <kernel/console.h>

extern console_t cons_table[];
extern console_t *curcons;	/* current active console */
extern unsigned short *vid_memory;
extern int nr_cons;

/* scroll up */
inline void scroll_screen(console_t *cons)
{
	memcpy((void*) cons->start, (void*) (cons->start + cons->width), (cons->heigth - 1) * cons->width * 2);
	memset((void*) (cons->start + ((cons->heigth-1) * cons->width)), 0, cons->width*2);
	cons->row = cons->heigth - 1;
}

void flush(console_t *cons)
{
	char *vid = (char*) cons->start;
	int i = 0, index = (cons->row * SCR_WIDTH) + cons->column;

	if (cons->count > 0) {	
		index <<= 1;
		while (cons->count-- > 0) {
			vid[index++] = cons->buf_queue[i];
			vid[index++] = cons->attr;
			i++; cons->column++;
		}
		cons->count = 0;
	}
	
	/* check cursor position */
	if (cons->column < 0) { cons->column = 79; cons->row--; }
	if (cons->row < 0) cons->row = 0;
	if (cons->column > (cons->width - 1)) { cons->column = 0; cons->row++; }
	if (cons->row > (cons->heigth - 1)) scroll_screen(cons);

	if (cons == curcons)
		update_cursor(cons);
}

/* output character on the specified console */
int out_char(console_t *cons, int c)
{
	switch (c)
	{
		case '\r':
			cons->column = 0;
			break;
		case '\n':	/* new line */
			cons->column = 0;
			cons->row++;
			break;
		case '\b':	/* backspace */
			if (cons->column == 0) {
				cons->row--;
				cons->column = (cons->width - 1);
			}
			else
				cons->column--;
			
			break;
		case '\t':	/* tabulator */
			cons->column += 8;
			if (cons->column > SCR_WIDTH) {
				cons->column -= SCR_WIDTH;
				cons->row++;
			}
			break;
		default:	/* it's an ordinary character */
			if (cons->count == BUF_QUEUE) flush(cons);
			cons->buf_queue[cons->count++] = c;
			
			break;
	}

	if (cons == curcons || cons->flush) 
		flush(cons);	/* avoid lagging */

	return 1;	
}

/* output safely using interrupts */
inline int putc(int c)
{
	int i;
	lock();
	
	curcons->attr = WHITE;
	i = out_char(curcons, c);
	
	unlock();
	return i;
}

/* orint a message on a specified console */
int printc(int cons_nr, const char *format, ...)
{
	console_t *cons;
	char buf[MAXLEN];
	va_list arg;
	int i = 0;

	lock();

	if (cons_nr < 0 || cons_nr > nr_cons)
		return -1;
	cons = &cons_table[cons_nr];
	
	va_start(arg, format);
	vsprintf(buf, format, arg);
	va_end(arg);

	while (buf[i] != '\0')
		out_char(cons, buf[i++]);
	unlock();
	return i;
}

/* print a message on the screen */
int printk(const char *format, ...)
{
	char buffer[MAXLEN];
	va_list arg;
	int i = 0;
	
	lock();	
	
	va_start(arg, format);
	vsprintf(buffer, format, arg);	
	va_end(arg);

	while (buffer[i] != '\0')
		out_char(curcons, buffer[i++]);
	
	unlock();
	return i;
}

/* screen output without modifying interrupts and without buffering output */
int debug(const char *s, ...)
{
	char buf[MAXLEN];
	va_list arg;
	int i = 0;

	va_start(arg, s);
	vsprintf(buf, s, arg);
	va_end(arg);

	while (buf[i] != '\0')
		out_char(curcons, buf[i++]);
	
	return i;
}

/* print an important message indicated by the flag parameter
 * on the screen */
int log(char *s, int flag)
{
	int i = 0;

	lock();
	
	curcons->attr = flag;
	while (s[i] != '\0')
		out_char(curcons, s[i++]);
	curcons->attr = WHITE;
	
	unlock();
	return i;
}

/* fatal error has happened */
void panic(const char *str, ...)
{
	char panic[] = "Kernel panic: ";
	char buf[MAXLEN];
	va_list arg;
	int i = 0;
	
	lock();
	va_start(arg, str);
	vsprintf(buf, str, arg);
	va_end(arg);
	
	curcons->attr = RED;	/* colour to use */
	while (panic[i] != '\0')
		out_char(curcons, panic[i++]);
	i = 0;
	curcons->attr = WHITE;
	
	while (buf[i] != '\0')
		out_char(curcons, buf[i++]);
	
	hlt();
}
