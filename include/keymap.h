/*
 * include/keymap.h
 * Copyright (C) 2002 Alexander Blessing
 * 
 * contains the keymap array
 */

#ifndef _INCLUDE_KEYMAP_H_
#define _INCLUDE_KEYMAP_H_

#define KBD_BUF 2

#define ESC 0x1B

#define KEY_F1 0x80
#define KEY_F2 (KEY_F1 + 1)
#define KEY_F3 (KEY_F2 + 1)
#define KEY_F4 (KEY_F3 + 1)
#define KEY_F5 (KEY_F4 + 1)
#define KEY_F6 (KEY_F5 + 1)
#define KEY_F7 (KEY_F6 + 1)
#define KEY_F8 (KEY_F7 + 1)
#define KEY_F9 (KEY_F8 + 1)
#define KEY_F10 (KEY_F9 + 1)
#define KEY_F11 (KEY_F10 + 1)
#define KEY_F12 (KEY_F11 + 1)

#define LSHIFT 0x2A
#define RSHIFT 0x36
#define CTRL 0x1D
#define ALT 0x38
#define DEL 0x53

#define KEY_INS 0x90
#define KEY_DEL (KEY_INS + 1) 
#define KEY_HOME (KEY_DEL + 1)
#define KEY_END (KEY_HOME + 1)
#define KEY_PGUP (KEY_END + 1)
#define KEY_PGDN (KEY_PGUP + 1)
#define KEY_LEFT (KEY_PGDN + 1)
#define KEY_UP (KEY_LEFT + 1)
#define KEY_DOWN (KEY_UP + 1)
#define KEY_RIGHT (KEY_DOWN + 1)

#define SYSRQ 0x70
#define PSCREEN (SYSRQ + 1)
#define KEY_WIN (PSCREEN + 1)
#define KEY_MENU (KEY_WIN + 1)

/* the keymap array
 * this here is important - it maps the scancodes to ASCII
 * we have one array for non-shifted and one for shifted
 * keys
 */
int keymap[][2] = {
	/* 0 */		{0, 0},		
	/* 1 */		{ESC, 0}, 
	/* 2 */		{'1', '!'}, 	
	/* 3 */		{'2', '@'}, 
	/* 4 */		{'3', '#'},
	/* 5 */		{'4', '$'},
	/* 6 */		{'5', '%'},
	/* 7 */		{'6', '^'},
	/* 8 */		{'7', '&'}, 
	/* 9 */		{'8', '*'}, 
	/* 10 */	{'9', '('}, 
	/* 11 */	{'0', ')'},
	/* 12 */	{'-', '_'}, 
	/* 13 */	{'=', '+'}, 
	/* 14 */	{'\b', '\b'}, 
	/* 15 */	{'\t', 0},
	/* 16 */	{'q', 'Q'}, 
	/* 17 */	{'w', 'W'}, 
	/* 18 */	{'e', 'E'},
	/* 19 */	{'r', 'R'},
	/* 20 */	{'t', 'T'},
	/* 21 */	{'z', 'Y'},
	/* 22 */	{'u', 'U'},
	/* 23 */	{'i', 'I'},
	/* 24 */	{'o', 'O'},
	/* 25 */	{'p', 'P'}, 
	/* 26 */	{'[', '{'}, 
	/* 27 */	{']', '}'},
	/* 28 */	{'\n', 0}, 
	/* 29 */	{CTRL, 0},
	/* 30 */	{'a', 'A'}, 
	/* 31 */	{'s', 'S'},
	/* 32 */	{'d', 'D'},
	/* 33 */	{'f', 'F'}, 
	/* 34 */	{'g', 'G'},
	/* 35 */	{'h', 'H'}, 
	/* 36 */	{'j', 'J'},
	/* 37 */	{'k', 'K'},
	/* 38 */	{'l', 'L'}, 
	/* 39 */	{';', ':'},
	/* 40 */	{'\'', '"'},
	/* 41 */	{'`', '~'}, 
	/* 42 */	{LSHIFT, 0}, 
	/* 43 */	{'\\', '|'},
	/* 44 */	{'z', 'Z'}, 
	/* 45 */	{'x', 'X'},
	/* 46 */	{'c', 'C'}, 
	/* 47 */	{'v', 'V'},
	/* 48 */	{'b', 'B'}, 
	/* 49 */	{'n', 'N'}, 
	/* 50 */	{'m', 'M'},
	/* 51 */	{',', ';'},
	/* 52 */	{'.', ':'},
	/* 53 */	{'/', '?'},
	/* 54 */	{RSHIFT, 0},
	/* 55 */	{0, 0},
	/* 56 */	{ALT, 0},
	/* 57 */	{' ', ' '},
	/* 58 */	{0, 0},
	/* 59 */	{KEY_F1, KEY_F1},
	/* 60 */	{KEY_F2, KEY_F2},
	/* 61 */	{KEY_F3, KEY_F3}, 
	/* 62 */	{KEY_F4, KEY_F4},
	/* 63 */	{KEY_F5, KEY_F5},
	/* 64 */	{KEY_F6, KEY_F6},
	/* 65 */	{KEY_F7, KEY_F7}, 
	/* 66 */	{KEY_F8, KEY_F8},
	/* 67 */	{KEY_F9, KEY_F9},
	/* 68 */	{KEY_F10, KEY_F10},
	/* 69 */	{0, 0}, 
	/* 70 */	{0, 0},
	/* 71 */	{KEY_HOME, '7'},
	/* 72 */	{KEY_UP, '8'},
	/* 73 */	{KEY_PGUP, '9'},
	/* 74 */	{'-', 0},
	/* 75 */	{KEY_LEFT, '4'},
	/* 76 */	{'5', 0},
	/* 77 */	{KEY_RIGHT, '6'},
	/* 78 */	{'+', 0},
	/* 79 */	{KEY_END, '1'},
	/* 80 */	{KEY_DOWN, '2'}, 
	/* 81 */	{KEY_PGDN, '3'},
	/* 82 */	{KEY_INS, '0'},
	/* 83 */	{KEY_DEL, KEY_DEL},
	/* 84 */	{KEY_F11, KEY_F11},
	/* 85 */	{KEY_F12, KEY_F12},
	/* 86 */	{0, 0},
	/* 87 */	{'\n', '\n'},
	/* 88 */	{CTRL, 0},
	/* 89 */	{'/', 0},
	/* 90 */	{SYSRQ, PSCREEN},
	/* 91 */	{ALT, 0},
	/* 92 */	{KEY_HOME, 0},
	/* 93 */	{KEY_UP, 0},
	/* 94 */	{KEY_PGUP, 0},
	/* 95 */	{KEY_LEFT, 0},
	/* 96 */	{KEY_RIGHT, 0},
	/* 97 */	{KEY_END, 0},
	/* 98 */	{KEY_DOWN, 0},
	/* 99 */	{KEY_PGDN, 0},
	/* 100 */	{KEY_INS, 0},
	/* 101 */	{KEY_DEL, 0},
	/* 102 */	{KEY_WIN, 0},
	/* 103 */	{KEY_WIN, 0},
	/* 104 */	{KEY_MENU, 0}
};

#endif
