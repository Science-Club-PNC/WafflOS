// This file is supposed to be able to be compiled. If it can't compile and execute, it's wrong.
// Compile it with: cc -std=c11 code_style.c -o code_style

// C-style comments with one space separation
#include <stdio.h>  // Inline comments begin 2 spaces after line (python style)
// Always include everything you use in your code, don't rely on other headers including that

//#include "code_style.h"  // Always include the header file
//#define __code_style_h__  // Header declarations are written like this

// Clear, descriptive (long if neccesary) variable/function names, no caps
static const char* potatoes = (char*)"POTATOES";
static const int fucks_given = -1;

// Indent with 4 spaces
enum asdf {
    qwert  // Same variable rules apply for enums
};

// Constants declared by the preprossesor in caps
#define PI_TIMES_A_LOT 15296

// One empty line between all top-level elements, except groups of variables
int main(/* C++ style comments when necessary. Mind the beginning and ending space */)
{  // Functions declared with a newline before opening brackets
    for (int i = 0 /* C99+ declaration of variables in for */; i < 10; i++) {
        // Everything else opens brackets in the same line
        // Comments are indented as needed
        printf("%i ", i);
    }

    if (1) {
        printf("\n");
    } else {
        // Else clauses are used on the same line as the ending bracket
    }
    if (0) {  // If there's a new if statement, give it a newline after the last bracket
        puts("Herpaderp");
    }

    // Whitespace is important
    return 0;
}
