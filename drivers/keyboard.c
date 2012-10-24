/*
 * driver/keyboard.c
 * Copyright (C) 2002 Alexander Blessing
 * 
 * handles keyboard stuff
 */

#include <stdio.h>
#include <string.h>
#include <kernel/kernel.h>
#include <kernel/console.h>
#include <kernel/interrupt.h>
#include <kernel/list.h>
#include <driver/keyboard.h>
#include <keymap.h>	/* for keymap array */

#define KEYMAP_SIZE (sizeof(keymap) / sizeof(keymap[0][0]))

struct list_head key_queue;

inline int scan_keyboard(void)
{
	int code;
	int ack;

	code = in(KBD_PORT);	/* get the code */
	
	/* ack the read of the code */
	ack = in(KBD_ACK);
	out(KBD_ACK, ack | 0x80);	/* high */
	out(KBD_ACK, ack);	/* low */

	return code;
}

/* map scan code to ASCII */
int map_key(int code)
{
	static int k_shift = 0;
	static int k_alt = 0;
	static int k_ctrl = 0;
	int key = 0;
	
	/* ignore invalid scan codes */
	if (code >= KEYMAP_SIZE)
		return 0;
	
	/* check if user released a key */
	if (code & KEY_RELEASED)
	{
		code &= 0x7F;
		switch(code)
		{
			case LSHIFT:
			case RSHIFT:
				k_shift = 0;
				return 0;
			case ALT:
				k_alt = 0;
				return 0;
			case CTRL:
				k_ctrl = 0;
			default:
				return 0;	/* drop - not interesting */
		}
	}
	
	/* check scan codes */
	switch(code)
	{
		case LSHIFT:
		case RSHIFT:
			k_shift = 1;
			return 0;
		case ALT:
			k_alt = 1;
			return 0;
		case CTRL:
			k_ctrl = 1;
			return 0;
		default:
			/* map scan codes to ASCII keys */
			if (k_shift)
				key = keymap[code][1];	/* big letters */
			else
				key = keymap[code][0];	/* small letters */
			break;
	}

	/* check if user wants to switch screens */
	if (k_shift) 
		switch(key) {
			case KEY_F1:
				switch_console(0);
				return 0;
			case KEY_F2:
				switch_console(1);
				return 0;
			case KEY_F3:
				switch_console(2);
				return 0;
			case KEY_F4:
				switch_console(3);
				return 0;
		}
			
	/* check if user wants to salute */
	if (k_ctrl && k_alt) {
		if (key == KEY_DEL)
			reboot();
		else if (key == ESC)
			halt();
	}

	return key;
}

/* keyboard handler called on every keyboard IRQ */
void kbd_handler(void)
{
	struct key key;
	int code = 0;

	/* we will read the scan code first
	 * next, we'll filter out those
	 * codes, which are not interesting for us
	 * at last, we'll map them to ASCII keys and
	 * store them in an array
	 */
	code = scan_keyboard();	/* get the key scan code */
	key.c = map_key(code);	/* map to ASCII */
	if (key.c == 0)	/* check if key is not implemented */
		return;
	list_queue_insert(&key_queue, &key, list_head); /* add key to circular queue */
}	

/* do some initializing */
void init_kbd(void)
{
	/* clear scan codes in the keyboard's internal buffer */
	while (in(KBD_CTRL) & 1)
		(void) scan_keyboard();
	
	LIST_HEAD_INIT(&key_queue);

	put_handler(0x21, (void*) &INT_IRQ1, GATE_DEFAULT);
	enable_irq(1);	/* ready to get keyboard interrupts */
}
