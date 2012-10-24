/*
 * include/stdarg.h
 * Copyright (C) 2002 Alexander Blessing
 * 
 * defines useful functions for retrieving a unknown amount
 * of parameters
 */

#ifndef _INCLUDE_STDARG_H_
#define _INCLUDE_STDARG_H_

/* these functions are from the official GNU stdarg.h file */

typedef char *va_list;

/* Amount of space required in an argument list for an arg of type TYPE.
   TYPE may alternatively be an expression whose type is used.  */

#define __va_rounded_size(TYPE)  \
  (((sizeof (TYPE) + sizeof (int) - 1) / sizeof (int)) * sizeof (int))

#define va_start(AP, LASTARG) \
 (AP = ((char *) &(LASTARG) + __va_rounded_size(LASTARG)))

extern void va_end (va_list);
#define va_end(AP) /* Nothing */

#define va_arg(AP, TYPE) (AP += __va_rounded_size (TYPE), \
  *((TYPE *) (AP - __va_rounded_size (TYPE))))

#endif
