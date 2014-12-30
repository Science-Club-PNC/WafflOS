#include <io.h>

#include "terminal.h"
#include "malloc.h"

#define OSNAME "WafflOS"

void load(char *str) {
    printf("[$eBUSY$r] %s", str);
}

void ok() {
    printf("\r[ $2OK$r ] \n");
}

void fail() {
    printf("\r[$4FAIL$r] \n");
}

void main()
{
    term_clear();

    printf("\n$!f$0Welcome$R to $!2"OSNAME"$!r!\n\n");

    load("Initializing heap");
    init_heap();
    ok();

    load("Loading whatever");
    // Do stuff
    ok();
    load("Loading something destined to fail miserably");
    // Do some error stuff
    fail();
    printf("\n$R$0 0 $1 1 $2 2 $3 3 $4 4 $5 5 $6 6 $7 7 $8 8 $9 9 $a a $b b $c c $d d $e e $f f ");
    printf("\n$R$!0 0 $!1 1 $!2 2 $!3 3 $!4 4 $!5 5 $!6 6 $!7 7 $!8 8 $!9 9 $!a a $!b b $!c c $!d d $!e e $!f f ");
    printf("$R\n\n");
    load("Loading something forever");
    printf("\n\n");
}
