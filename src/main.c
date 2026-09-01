#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

#define MAX_SOURCE_SIZE 10000

int main(int argc, char *argv[]) {

    FILE *file;
    char source[MAX_SOURCE_SIZE];
    size_t length;

    printf("========================================\n");
    printf("   TEAM 10 - MINI COMPILER\n");
    printf("   BCSE307L - COMPILER DESIGN\n");
    printf("========================================\n\n");

    /* Check command-line argument */
    if (argc != 2) {
        printf("Usage: %s <test_file>\n", argv[0]);
        printf("\nExample:\n");
        printf("  %s tests/test_assignment.c\n", argv[0]);
        return EXIT_FAILURE;
    }

    /* Open test file */
    file = fopen(argv[1], "r");

    if (file == NULL) {
        printf("Error: Cannot open file '%s'\n", argv[1]);
        return EXIT_FAILURE;
    }

    /* Read source program */
    length = fread(source, 1, MAX_SOURCE_SIZE - 1, file);
    source[length] = '\0';

    fclose(file);

    printf("Input Program:\n");
    printf("%s\n", source);

    printf("Starting compilation...\n");

    parser_init(source);
    parse_program();

    printf("Compilation successful.\n");

    print_tac();

    return EXIT_SUCCESS;
}