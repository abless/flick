/* 
 * include/driver/keyboard.h
 * Copyright (C) 2002 Alexander Blessing
 */

#ifndef _INCLUDE_DRIVER_KEYBOARD_H_
#define _INCLUDE_DRIVER_KEYBOARD_H_

#include <kernel/list.h>

#define KEY_RELEASED 0x80

#define KBD_PORT 0x60
#define KBD_CTRL 0x64
#define KBD_ACK 0x61

struct key {
	int c;	/* character */
	struct list_head list_head;	/* for queue */
};

void init_kbd(void);

#endif
