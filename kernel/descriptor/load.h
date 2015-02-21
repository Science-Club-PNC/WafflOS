#ifndef __load_h__
#define __load_h__

#include <stdint.h>

void lgdt(void* ptr);
void lidt(void* ptr);

#endif
