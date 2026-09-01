#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

static Token current_token;

static TAC code[MAX_CODE];
static int code_count = 0;

static Symbol symbol_table[MAX_SYMBOLS];
static int symbol_count = 0;

static int temp_count = 0;

/* ---------- Utility Functions ---------- */

static void error(const char *message) {
    fprintf(stderr,
            "Syntax/Semantic Error at line %d, column %d: %s\n",
            current_token.line,
            current_token.column,
            message);

    exit(EXIT_FAILURE);
}

static void advance(void) {
    current_token = get_next_token();
}

static void expect(TokenType type) {
    if (current_token.type != type) {
        error("Unexpected token");
    }

    advance();
}

/* ---------- Symbol Table ---------- */

static int is_declared(const char *name) {
    for (int i = 0; i < symbol_count; i++) {
        if (strcmp(symbol_table[i].name, name) == 0) {
            return 1;
        }
    }

    return 0;
}

static void declare_symbol(const char *name) {

    if (is_declared(name)) {
        error("Variable already declared");
    }

    if (symbol_count >= MAX_SYMBOLS) {
        error("Symbol table full");
    }

    strcpy(symbol_table[symbol_count].name, name);
    symbol_count++;
}

/* ---------- Temporary Variables ---------- */

static char *new_temp(void) {

    static char temp[64];

    snprintf(temp, sizeof(temp),
             "t%d", temp_count++);

    return temp;
}

/* ---------- Three Address Code ---------- */

static int emit(const char *op,
                const char *arg1,
                const char *arg2,
                const char *result) {

    if (code_count >= MAX_CODE) {
        error("TAC buffer full");
    }

    strcpy(code[code_count].op, op);
    strcpy(code[code_count].arg1, arg1);
    strcpy(code[code_count].arg2, arg2);
    strcpy(code[code_count].result, result);

    return code_count++;
}

/* ---------- Expressions ---------- */

static ExprAttr parse_expression(void);

static ExprAttr parse_factor(void) {

    ExprAttr result;

    if (current_token.type == TOKEN_ID) {

        if (!is_declared(current_token.lexeme)) {
            error("Undeclared variable");
        }

        strcpy(result.place, current_token.lexeme);

        advance();
        return result;
    }

    if (current_token.type == TOKEN_NUMBER) {

        strcpy(result.place, current_token.lexeme);

        advance();
        return result;
    }

    if (current_token.type == TOKEN_LPAREN) {

        advance();

        result = parse_expression();

        expect(TOKEN_RPAREN);

        return result;
    }

    error("Expected identifier, number or expression");

    return result;
}

static ExprAttr parse_term(void) {

    ExprAttr left = parse_factor();

    while (current_token.type == TOKEN_MUL ||
           current_token.type == TOKEN_DIV) {

        TokenType operator = current_token.type;

        advance();

        ExprAttr right = parse_factor();

        char *temp = new_temp();

        if (operator == TOKEN_MUL) {
            emit("*",
                 left.place,
                 right.place,
                 temp);
        } else {
            emit("/",
                 left.place,
                 right.place,
                 temp);
        }

        strcpy(left.place, temp);
    }

    return left;
}

static ExprAttr parse_expression(void) {

    ExprAttr left = parse_term();

    while (current_token.type == TOKEN_PLUS ||
           current_token.type == TOKEN_MINUS) {

        TokenType operator = current_token.type;

        advance();

        ExprAttr right = parse_term();

        char *temp = new_temp();

        if (operator == TOKEN_PLUS) {
            emit("+",
                 left.place,
                 right.place,
                 temp);
        } else {
            emit("-",
                 left.place,
                 right.place,
                 temp);
        }

        strcpy(left.place, temp);
    }

    return left;
}

/* ---------- Assignment ---------- */

static void parse_assignment(void) {

    char variable[64];

    strcpy(variable, current_token.lexeme);

    if (!is_declared(variable)) {
        error("Undeclared variable");
    }

    expect(TOKEN_ID);
    expect(TOKEN_ASSIGN);

    ExprAttr expression = parse_expression();

    emit("=",
         expression.place,
         "",
         variable);

    expect(TOKEN_SEMICOLON);
}

/* ---------- Declaration ---------- */

static void parse_declaration(void) {

    expect(TOKEN_INT);

    if (current_token.type != TOKEN_ID) {
        error("Expected identifier");
    }

    while (1) {

        declare_symbol(current_token.lexeme);

        expect(TOKEN_ID);

        if (current_token.type != TOKEN_COMMA) {
            break;
        }

        advance();
    }

    expect(TOKEN_SEMICOLON);
}

/* ---------- Program ---------- */

void parser_init(const char *source) {

    lexer_init(source);

    current_token = get_next_token();

    code_count = 0;
    symbol_count = 0;
    temp_count = 0;
}

void parse_program(void) {

    while (current_token.type != TOKEN_EOF) {

        if (current_token.type == TOKEN_INT) {

            parse_declaration();

        } else if (current_token.type == TOKEN_ID) {

            parse_assignment();

        } else {

            error("Unexpected statement");
        }
    }
}

/* ---------- TAC Output ---------- */

void print_tac(void) {

    printf("\n=== THREE ADDRESS CODE ===\n\n");

    for (int i = 0; i < code_count; i++) {

        if (strcmp(code[i].op, "=") == 0) {

            printf("%d: %s = %s\n",
                   i,
                   code[i].result,
                   code[i].arg1);

        } else {

            printf("%d: %s = %s %s %s\n",
                   i,
                   code[i].result,
                   code[i].arg1,
                   code[i].op,
                   code[i].arg2);
        }
    }
}