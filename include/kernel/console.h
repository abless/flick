/*
 * include/kernel/console.h
 * Copyright (C) 2003 Alexander Blessing
 */

#ifndef _INCLUDE_KERNEL_CONSOLE_H_
#define _INCLUDE_KERNEL_CONSOLE_H_

#define BYTE 0xFF
#define BUF_QUEUE 150

#define SCR_WIDTH 80
#define SCR_SIZE (80 * 25)

#define VGA_MISC 0x3CC
#define IS_COLOUR 0x01

#define VID_MONO 0xB000	/* monochrome video memory */
#define MONO_SIZE 0x1000
#define VID_COLOUR 0xB8000	/* colour video memory */
#define COLOUR_SIZE 0x4000
#define CRTC_MONO 0x3B4	/* monochrome CRTC port */
#define CRTC_COLOUR 0x3D4	/* colour CRTC port */

/* tty structure */
typedef struct {
	int column;	/* x */
	int row;	/* y */
	int buf_queue[BUF_QUEUE];	/* character printed on this console */
	int count;
	int flush;	/* flag indicating wether to flush output */
	unsigned int nr;
	unsigned int attr;
	unsigned int heigth;
	unsigned int width;
	unsigned short *start;
	unsigned short *banner_start;	/* start of banner */
} console_t;

/* prototypes */
void switch_console(int);
void set_vid(int, int);
void init_tty(void);
void update_cursor(console_t *);

#endif
