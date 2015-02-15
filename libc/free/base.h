#ifndef __base_h__
#define __base_h__

#include <io.h>
#include <kernel/ioport.h>

// Some centeralised basic information.
#define OSNAME "WafflOS"
#define OSVERSION "0.2.0"

// If not compiling in GNU C omit __attribute__().
#ifndef __GNUC__
#define __attribute__(x)
#endif

// outputs a string to the bochs debug console
#define bochs_print(str) {size_t i = 0; while(str[i]) {outb(0xe9, str[i]); i++;}}
// stops simulation and breaks into the debug console
#define bochs_break() outw(0x8A00,0x8A00); outw(0x8A00,0x08AE0);

// Print debug message. can be disabled by defining NDEBUG.
#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) printf("$RDEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

// Log an error with the file name and line number.
#define log_err(M, ...) printf("$!r$c   ERROR$r: $7(%s:%i)$r " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// Log a warning with the file name and line number.
#define log_warn(M, ...) printf("$!r$d WARNING$r: $7(%s:%i)$r " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// Log info with the file name and line number.
#define log_info(M, ...) printf("$!r$9    INFO$r: $7(%s:%i)$r " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

// Log an error and jump to 'error' if A isn't true.
#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// Log an error and jump to 'error'. Should be used if code is never meant to be reached.
#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

// Checks if pointer A is valid otherwise, log an error and jump to 'error'.
#define check_mem(A) check((A), "Out of memory.")

// Log an error and jump to 'error' if A isn't true. Can be silenced with NDEBUG.
#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

#endif

