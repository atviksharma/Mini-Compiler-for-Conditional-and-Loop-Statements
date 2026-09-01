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

/* ---------- Backpatching ---------- */

static void add_to_list(int list[],
                        int *count,
                        int index) {

    if (*count >= MAX_LIST) {
        error("Backpatch list full");
    }

    list[*count] = index;
    (*count)++;
}

static void merge_lists(int destination[],
                        int *destination_count,
                        int source[],
                        int source_count) {

    for (int i = 0; i < source_count; i++) {

        add_to_list(destination,
                    destination_count,
                    source[i]);
    }
}

static void backpatch(int list[],
                      int count,
                      int target) {

    char target_string[32];

    snprintf(target_string,
             sizeof(target_string),
             "%d",
             target);

    for (int i = 0; i < count; i++) {

        strcpy(code[list[i]].result,
               target_string);
    }
}

/* ---------- Expressions ---------- */

static ExprAttr parse_expression(void);

static ExprAttr parse_factor(void) {

    ExprAttr result;

    if (current_token.type == TOKEN_ID) {

        if (!is_declared(current_token.lexeme)) {
            error("Undeclared variable");
        }

        strcpy(result.place,
               current_token.lexeme);

        advance();

        return result;
    }

    if (current_token.type == TOKEN_NUMBER) {

        strcpy(result.place,
               current_token.lexeme);

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

/* ---------- Boolean Expressions ---------- */

static BoolAttr parse_boolean_expression(void);

static BoolAttr parse_relational(void) {

    BoolAttr result;

    result.true_count = 0;
    result.false_count = 0;

    ExprAttr left = parse_expression();

    TokenType operator = current_token.type;

    if (operator != TOKEN_LT &&
        operator != TOKEN_GT &&
        operator != TOKEN_LE &&
        operator != TOKEN_GE &&
        operator != TOKEN_EQ &&
        operator != TOKEN_NE) {

        error("Expected relational operator");
    }

    advance();

    ExprAttr right = parse_expression();

    int true_index;
    int false_index;

    switch (operator) {

        case TOKEN_LT:

            true_index = emit("if<",
                              left.place,
                              right.place,
                              "");
            break;

        case TOKEN_GT:

            true_index = emit("if>",
                              left.place,
                              right.place,
                              "");
            break;

        case TOKEN_LE:

            true_index = emit("if<=",
                              left.place,
                              right.place,
                              "");
            break;

        case TOKEN_GE:

            true_index = emit("if>=",
                              left.place,
                              right.place,
                              "");
            break;

        case TOKEN_EQ:

            true_index = emit("if==",
                              left.place,
                              right.place,
                              "");
            break;

        case TOKEN_NE:

            true_index = emit("if!=",
                              left.place,
                              right.place,
                              "");
            break;

        default:

            error("Invalid relational operator");
    }

    false_index = emit("goto",
                        "",
                        "",
                        "");

    add_to_list(result.truelist,
                &result.true_count,
                true_index);

    add_to_list(result.falselist,
                &result.false_count,
                false_index);

    return result;
}

static BoolAttr parse_boolean_and(void) {

    BoolAttr left = parse_relational();

    while (current_token.type == TOKEN_AND) {

        advance();

        int marker = code_count;

        BoolAttr right = parse_relational();

        backpatch(left.truelist,
                  left.true_count,
                  marker);

        BoolAttr result;

        result.true_count = 0;
        result.false_count = 0;

        merge_lists(result.truelist,
                    &result.true_count,
                    right.truelist,
                    right.true_count);

        merge_lists(result.falselist,
                    &result.false_count,
                    left.falselist,
                    left.false_count);

        merge_lists(result.falselist,
                    &result.false_count,
                    right.falselist,
                    right.false_count);

        left = result;
    }

    return left;
}

static BoolAttr parse_boolean_expression(void) {

    BoolAttr left = parse_boolean_and();

    while (current_token.type == TOKEN_OR) {

        advance();

        int marker = code_count;

        BoolAttr right = parse_boolean_and();

        backpatch(left.falselist,
                  left.false_count,
                  marker);

        BoolAttr result;

        result.true_count = 0;
        result.false_count = 0;

        merge_lists(result.truelist,
                    &result.true_count,
                    left.truelist,
                    left.true_count);

        merge_lists(result.truelist,
                    &result.true_count,
                    right.truelist,
                    right.true_count);

        merge_lists(result.falselist,
                    &result.false_count,
                    right.falselist,
                    right.false_count);

        left = result;
    }

    return left;
}

/* ---------- Statements ---------- */

static void parse_statement(void);

static void parse_block(void) {

    expect(TOKEN_LBRACE);

    while (current_token.type != TOKEN_RBRACE &&
           current_token.type != TOKEN_EOF) {

        parse_statement();
    }

    expect(TOKEN_RBRACE);
}

/* ---------- Assignment ---------- */

static void parse_assignment(void) {

    char variable[64];

    strcpy(variable,
           current_token.lexeme);

    if (!is_declared(variable)) {
        error("Undeclared variable");
    }

    expect(TOKEN_ID);

    expect(TOKEN_ASSIGN);

    ExprAttr expression =
        parse_expression();

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

/* ---------- IF Statement ---------- */

static void parse_if(void) {

    expect(TOKEN_IF);

    expect(TOKEN_LPAREN);

    BoolAttr condition =
        parse_boolean_expression();

    expect(TOKEN_RPAREN);

    int then_start = code_count;

    backpatch(condition.truelist,
              condition.true_count,
              then_start);

    parse_statement();

    if (current_token.type == TOKEN_ELSE) {

        int jump_to_end =
            emit("goto",
                 "",
                 "",
                 "");

        int else_start = code_count;

        backpatch(condition.falselist,
                  condition.false_count,
                  else_start);

        advance();

        parse_statement();

        int end = code_count;

        int end_list[1];

        end_list[0] = jump_to_end;

        backpatch(end_list,
                  1,
                  end);

    } else {

        int end = code_count;

        backpatch(condition.falselist,
                  condition.false_count,
                  end);
    }
}

/* ---------- WHILE Statement ---------- */

static void parse_while(void) {

    expect(TOKEN_WHILE);

    int condition_start = code_count;

    expect(TOKEN_LPAREN);

    BoolAttr condition =
        parse_boolean_expression();

    expect(TOKEN_RPAREN);

    int body_start = code_count;

    backpatch(condition.truelist,
              condition.true_count,
              body_start);

    parse_statement();

    char target[32];

    snprintf(target,
             sizeof(target),
             "%d",
             condition_start);

    emit("goto",
         "",
         "",
         target);

    int end = code_count;

    backpatch(condition.falselist,
              condition.false_count,
              end);
}

/* ---------- Statement Dispatcher ---------- */

static void parse_statement(void) {

    if (current_token.type == TOKEN_INT) {

        parse_declaration();

    } else if (current_token.type == TOKEN_ID) {

        parse_assignment();

    } else if (current_token.type == TOKEN_IF) {

        parse_if();

    } else if (current_token.type == TOKEN_WHILE) {

        parse_while();

    } else if (current_token.type == TOKEN_LBRACE) {

        parse_block();

    } else {

        error("Unexpected statement");
    }
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

        parse_statement();
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

        } else if (strcmp(code[i].op, "goto") == 0) {

            printf("%d: goto %s\n",
                   i,
                   code[i].result);

        } else if (strncmp(code[i].op, "if", 2) == 0) {

            printf("%d: %s %s, %s goto %s\n",
                   i,
                   code[i].op,
                   code[i].arg1,
                   code[i].arg2,
                   code[i].result);

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