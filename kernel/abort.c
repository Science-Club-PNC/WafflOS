#include <base.h>
#include <io.h>

#include "abort.h"

void __attribute__((noreturn)) abort(char* msg)
{
    // Print abort message
    printf("\n$R$0$!4[ABORT]$R %s", msg);

    // Halt CPU
    __asm__("cli");
    __asm__("hlt");

    // Hang CPU if halt fails
    loop:
    goto loop;
}
