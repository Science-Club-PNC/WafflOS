#ifndef _VARARG_H
#define _VARARG_H

typedef void** va_list;

#define va_start(list, last) list = (va_list*)&last + 1
#define va_arg(list, type) *(type*)list++

#endif
