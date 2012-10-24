/*
 * include/string.h
 * Copyright (C) 2002 Alexander Blessing
 * 
 * contains declarations of string functions defined in lib/string.c
 */

#ifndef _INCLUDE_STRING_H_
#define _INCLUDE_STRING_H_

#include <types.h>

void itoa(unsigned long, unsigned char *, char);

size_t strlen(const char *);
int strcmp(const char *, const char *);
int strncmp(const char *, const char *, size_t);
char *strcat(char *, const char *);
char *strncat(char *, const char*, size_t);
char *strcpy(char *, const char *);
char *strncpy(char *, const char*, size_t);
void *memcpy(void *, const void *, size_t);
void *memset(void *, int, size_t );
#endif
