#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

int main(void) {

    const char *source =
    "int a, b, x;"
    "a = 0;"
    "b = 5;"
    "while (a < b) {"
    "if (a == 2) {"
    "x = a + b;"
    "} else {"
    "x = a + 1;"
    "}"
    "a = a + 1;"
    "}";

    printf("========================================\n");
    printf("   TEAM 10 - MINI COMPILER\n");
    printf("   BCSE307L - COMPILER DESIGN\n");
    printf("========================================\n\n");

    printf("Input Program:\n");
    printf("%s\n\n", source);

    printf("Starting compilation...\n");

    parser_init(source);
    parse_program();

    printf("Compilation successful.\n");

    print_tac();

    return 0;
}
