/*
 * include/types.h
 * Copyright (C) 2002 Alexander Blessing
 *
 * defines our data types
 */

#ifndef _INCLUDE_TYPES_H_
#define _INCLUDE_TYPES_H_

#ifndef NULL
#define NULL (void*) 0 
#endif

typedef unsigned int semaphore;	/* for mutex */
typedef unsigned int size_t;
typedef signed int ssize_t;
typedef unsigned long addr_t;
typedef int pid_t;

#endif
