/*
 * kernel/console.c
 * Copyright (C) 2003 Alexander Blessing
 */

#include <stdio.h>
#include <string.h>
#include <kernel/kernel.h>
#include <kernel/console.h>

#define LINE 80

/* console width and heigth from BIOS */
#define CONS_WIDTH (*(unsigned short *) 0x44A)
#define CONS_HEIGTH ((*(unsigned char *)0x484) + 1)

#define MAX_CONS 4	/* don't want more */
#define VID_ORG 12
#define CURSOR 14

#define CRTC_INDEX 0
#define CRTC_DATA 1 

/* console banner structure */
struct {
	char *banner;
	int attr;
} banner_table[] = { 
	{ "Welcome to Flick - console #1", (BLUE << 4) | (WHITE | INTENSITY) },	
	{ "Welcome to Flick - console #2", (GREEN << 4) | (WHITE | INTENSITY) },
	{ "Welcome to Flick - console #3", (RED << 4) | (WHITE | INTENSITY) },
	{ "Welcome to Flick - console #4", (MAGENTA << 4) | (WHITE | INTENSITY) }
};

console_t *curcons = NULL, cons_table[MAX_CONS];
unsigned short *vid_memory;
int vid_port, nr_cons;

void update_cursor(console_t *cons)
{
	unsigned short cur;
	
	/* only update if console = curcons */
	if (cons != curcons)
		return;

	cur = (cons->start - vid_memory) + (cons->row * SCR_WIDTH) + cons->column;
	
	out(vid_port + CRTC_INDEX, CURSOR);
	out(vid_port + CRTC_DATA, (cur >> 8) & BYTE);
	out(vid_port + CRTC_INDEX, CURSOR + 1);
	out(vid_port + CRTC_DATA, cur & BYTE);
}

/* clear current screen */
inline void clear(void)
{
	char *addr = (char*) curcons->start;
	int n = 0;

	while (n < (SCR_SIZE * 2)) {
		addr[n++] = ' ';
		addr[n++] = WHITE;
	}
	curcons->column = curcons->row = 0;
	update_cursor(curcons);
}

/* clear screen indicated by cons_nr */
inline void clear_screen(int cons_nr)
{
	console_t *cons;
	char *addr;
	int n = 0;
	
	if (cons_nr < 0 || cons_nr > nr_cons)
		return;
	
	/* clear specified screen */
	cons = &cons_table[cons_nr];
	addr = (char*) cons->banner_start;
	
	while (n < (SCR_SIZE * 2)) {
		addr[n++] = ' ';
		addr[n++] = WHITE;
	}
	cons->row = cons->column = 0;
	update_cursor(cons);	/* only updated if current console is active */	
}

void switch_console(int cons_nr)
{
	unsigned int addr;
	
	/* switch to console number cons_nr */
	if (cons_nr >= nr_cons || cons_nr < 0)
		return;

	/* flush buffer from old console */
	flush(curcons);
	
	curcons = &cons_table[cons_nr];
	addr = (unsigned int) (curcons->banner_start - vid_memory);

	/* program 6845 chip */
	out(vid_port + CRTC_INDEX, VID_ORG);
	out(vid_port + CRTC_DATA, (addr >> 8) & BYTE);
	out(vid_port + CRTC_INDEX, VID_ORG + 1);
	out(vid_port + CRTC_DATA, addr & BYTE);
	
	update_cursor(curcons);
	return;
}

void setup_banner(int cons_nr)
{
	console_t *cons;
	char *p, *addr;
	int attr, n, index;

	if (cons_nr < 0 || cons_nr > nr_cons)
		return;
	
	/* we just want some nice looking banner/title
	 * on top of every (virtual) console
	 * banners get one line on top and all have a
	 * different colour
	 */
	cons = &cons_table[cons_nr];
	p = banner_table[cons_nr].banner;
	attr = banner_table[cons_nr].attr;
	
	if ((n = strlen(p)) > cons->width)
		return;
	
	addr = (char*) cons->banner_start;
	
	/* fill line with colour first */
	for (index = 0; index < (cons->width * 2); ) {
		addr[index++] = ' ';
		addr[index++] = attr;
	}
	/* now print the line */
	for (index = 0; n > 0; n--) {
		addr[index++] = *(p++);
		addr[index++] = attr;
	}
}		
	
inline void init_console(int cons_nr)
{
	/* initialize console attributes */
	cons_table[cons_nr].nr = cons_nr;
	cons_table[cons_nr].width = CONS_WIDTH;
	cons_table[cons_nr].heigth = CONS_HEIGTH - 1;	/* because of banner */
	cons_table[cons_nr].attr = WHITE;
	cons_table[cons_nr].flush = 1;
	cons_table[cons_nr].count = cons_table[cons_nr].row = cons_table[cons_nr].column = 0;
	cons_table[cons_nr].banner_start = (unsigned short *) (vid_memory + (2 * cons_nr * SCR_SIZE));
	cons_table[cons_nr].start = (unsigned short*) (LINE + cons_table[cons_nr].banner_start);
	
	clear_screen(cons_nr);
	setup_banner(cons_nr);	/* setup banner title for console */
}

void init_screen(void)
{
	int i;
	
	/* get video I/O port from BIOS */
	if (in(VGA_MISC) & IS_COLOUR) {	/* colour */
		vid_port = CRTC_COLOUR;
		vid_memory = (unsigned short*) VID_COLOUR;
		nr_cons = COLOUR_SIZE / SCR_SIZE;			
	}	
	else {	/* monochrome */
		vid_port = CRTC_MONO;
		vid_memory = (unsigned short*) VID_MONO;
		nr_cons = MONO_SIZE / SCR_SIZE;
	}
	
	if (nr_cons > MAX_CONS)
		nr_cons = MAX_CONS;
	curcons = &cons_table[0];	/* select default console */

	for (i = 0; i < nr_cons; i++) 
		init_console(i);
	
	switch_console(0);	/* switch to first console */
}
