#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"

#define MAX_CODE 500
#define MAX_SYMBOLS 100
#define MAX_LIST 100

typedef struct {
    char name[MAX_LEXEME];
} Symbol;

typedef struct {
    char op[20];
    char arg1[64];
    char arg2[64];
    char result[64];
} TAC;

typedef struct {
    char place[64];
} ExprAttr;

typedef struct {
    int truelist[MAX_LIST];
    int falselist[MAX_LIST];
    int true_count;
    int false_count;
} BoolAttr;

void parser_init(const char *source);
void parse_program(void);
void print_tac(void);

#endif