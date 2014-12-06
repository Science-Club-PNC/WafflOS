#ifndef __vararg_h__
#define __vararg_h__

typedef void **va_list;

#define va_start(list, last) list = (va_list *)&last + 1
#define va_arg(list, type) *(type *)list++

#endif
