#include <base.h>
#include <io.h>

#include "terminal.h"
#include "malloc.h"

void load(char *str) {
    printf("[$eBUSY$r] %s", str);
}

void ok() {
    printf("\r[ $aOK$r ] \n");
}

void fail() {
    printf("\r[$cFAIL$r] \n");
}

void main()
{
    term_clear();

    printf("\n$!f$0Welcome$R to $!2"OSNAME"$!r version "OSVERSION"!\n\n");

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
    log_err("WE FAILED");
    log_warn("is this a problem?");
    log_info("worthless info! :D");
    load("Loading something forever");
    printf("\n\n");
}
