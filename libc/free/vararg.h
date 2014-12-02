#ifndef __vararg_h__
#define __vararg_h__

typedef void *va_list;

#define va_start(list, last) (list = (void *)&last + sizeof(last))
#define va_arg(list, type) (list += sizeof(type), *(type *)(list - sizeof(type)))

#endif
