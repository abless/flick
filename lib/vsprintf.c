/*
 * lib/vsprintf.c
 * Copyright (C) 2002 Alexander Blessing
 *
 * this file only contains the quite important function vsprintf.c
 * This function is totally written by me - not as powerful as Linus'
 * but it's my own one :-)
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>	/* for va_list and va_arg */
#include <kernel/interrupt.h>

/* vsprintf() - dissolve conversions */
void vsprintf(char *str, const char *format, va_list ap)
{
	char *p = str;
	char *s; int d; char c; char buf[MAXLEN];
	int i = 0;
	
	*p = '\0';
	/* go through format until we reach a % */
	while (*format != '\0')
	{
		if (*format != '%') {	/* ordinary character */
			i = strlen(p);
			p[i] = *format;
			p[i + 1] = '\0';
		}
		
		else {
			switch (*(format + 1)) {
				case 'c':	/* char */
					c = (char) va_arg(ap, int);
					i = strlen(p);
					p[i] = c;
					p[i + 1]  = '\0';
					break;		
				case 's':	/* string */
					s = va_arg(ap, char *);
					strcat(p, s);
					break;
				case 'd':	/* integer */
				case 'i':
					d = va_arg(ap, int);
					itoa(d, buf, 10);	/* convert to string */
					strcat(p, buf);
					break;
				case 'x':	/* hexa-decimal */
					d = va_arg(ap, int);
					itoa(d, buf, 16);
					strcat(p, buf);
					break;
				default:
					/* print the % and the following character */
					i = strlen(p);
					p[i]  = *format;
					p[i + 1]  = *(format + 1);
					p[i + 2]  = '\0';
					break;
			}
			format++;
		}
		format++;
	}
}				
