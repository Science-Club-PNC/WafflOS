// C-style comments with one space separation
#include <stdio.h>  // Inline comments begin 2 spaces after line (python style)

// Clear, descriptive (long if neccesary) variable/function names, no caps.
static const char *potatoes = (char *)"POTATOES";
static const int fucks_given = -1;

// Indent with 4 spaces
enum asdf {
    qwert  // Same goes for enums
};

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

// TODO: Expand/change this accordingly.
