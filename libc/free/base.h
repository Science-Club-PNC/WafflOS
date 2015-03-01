#ifndef __base_h__
#define __base_h__

// Some centeralised basic information.
#define OSNAME "WafflOS"
#define OSVERSION "0.2.0"

// If not compiling in GNU C omit __attribute__().
#ifndef __GNUC__
#define __attribute__(x)
#warning "this OS is made for GNU C. compiling in any other compiler probably won't work"
#endif

#endif
