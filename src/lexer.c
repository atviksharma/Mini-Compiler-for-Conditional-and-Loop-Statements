#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

static const char *input;
static int pos;
static int line;
static int column;

void lexer_init(const char *source) {
    input = source;
    pos = 0;
    line = 1;
    column = 1;
}

static void advance_char(void) {
    if (input[pos] == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }

    pos++;
}

static Token make_token(TokenType type, const char *lexeme,
                        int token_line, int token_column) {
    Token token;

    token.type = type;
    strncpy(token.lexeme, lexeme, MAX_LEXEME - 1);
    token.lexeme[MAX_LEXEME - 1] = '\0';

    token.line = token_line;
    token.column = token_column;

    return token;
}

Token get_next_token(void) {
    char buffer[MAX_LEXEME];
    int start_line;
    int start_column;
    int i = 0;

    while (isspace((unsigned char)input[pos])) {
        advance_char();
    }

    start_line = line;
    start_column = column;

    if (input[pos] == '\0') {
        return make_token(TOKEN_EOF, "EOF",
                          start_line, start_column);
    }

    if (isalpha((unsigned char)input[pos]) ||
        input[pos] == '_') {

        while (isalnum((unsigned char)input[pos]) ||
               input[pos] == '_') {

            if (i < MAX_LEXEME - 1) {
                buffer[i++] = input[pos];
            }

            advance_char();
        }

        buffer[i] = '\0';

        if (strcmp(buffer, "int") == 0)
            return make_token(TOKEN_INT, buffer,
                              start_line, start_column);

        if (strcmp(buffer, "if") == 0)
            return make_token(TOKEN_IF, buffer,
                              start_line, start_column);

        if (strcmp(buffer, "else") == 0)
            return make_token(TOKEN_ELSE, buffer,
                              start_line, start_column);

        if (strcmp(buffer, "while") == 0)
            return make_token(TOKEN_WHILE, buffer,
                              start_line, start_column);

        return make_token(TOKEN_ID, buffer,
                          start_line, start_column);
    }

    if (isdigit((unsigned char)input[pos])) {

        while (isdigit((unsigned char)input[pos])) {

            if (i < MAX_LEXEME - 1) {
                buffer[i++] = input[pos];
            }

            advance_char();
        }

        buffer[i] = '\0';

        return make_token(TOKEN_NUMBER, buffer,
                          start_line, start_column);
    }

    if (input[pos] == '<' && input[pos + 1] == '=') {
        advance_char();
        advance_char();
        return make_token(TOKEN_LE, "<=",
                          start_line, start_column);
    }

    if (input[pos] == '>' && input[pos + 1] == '=') {
        advance_char();
        advance_char();
        return make_token(TOKEN_GE, ">=",
                          start_line, start_column);
    }

    if (input[pos] == '=' && input[pos + 1] == '=') {
        advance_char();
        advance_char();
        return make_token(TOKEN_EQ, "==",
                          start_line, start_column);
    }

    if (input[pos] == '!' && input[pos + 1] == '=') {
        advance_char();
        advance_char();
        return make_token(TOKEN_NE, "!=",
                          start_line, start_column);
    }

    if (input[pos] == '&' && input[pos + 1] == '&') {
        advance_char();
        advance_char();
        return make_token(TOKEN_AND, "&&",
                          start_line, start_column);
    }

    if (input[pos] == '|' && input[pos + 1] == '|') {
        advance_char();
        advance_char();
        return make_token(TOKEN_OR, "||",
                          start_line, start_column);
    }

    {
        char current[2];

        current[0] = input[pos];
        current[1] = '\0';

        advance_char();

        switch (current[0]) {
            case '+':
                return make_token(TOKEN_PLUS, current,
                                  start_line, start_column);

            case '-':
                return make_token(TOKEN_MINUS, current,
                                  start_line, start_column);

            case '*':
                return make_token(TOKEN_MUL, current,
                                  start_line, start_column);

            case '/':
                return make_token(TOKEN_DIV, current,
                                  start_line, start_column);

            case '=':
                return make_token(TOKEN_ASSIGN, current,
                                  start_line, start_column);

            case '<':
                return make_token(TOKEN_LT, current,
                                  start_line, start_column);

            case '>':
                return make_token(TOKEN_GT, current,
                                  start_line, start_column);

            case '(':
                return make_token(TOKEN_LPAREN, current,
                                  start_line, start_column);

            case ')':
                return make_token(TOKEN_RPAREN, current,
                                  start_line, start_column);

            case '{':
                return make_token(TOKEN_LBRACE, current,
                                  start_line, start_column);

            case '}':
                return make_token(TOKEN_RBRACE, current,
                                  start_line, start_column);

            case ';':
                return make_token(TOKEN_SEMICOLON, current,
                                  start_line, start_column);

            case ',':
                return make_token(TOKEN_COMMA, current,
                                  start_line, start_column);

            default:
                return make_token(TOKEN_ERROR, current,
                                  start_line, start_column);
        }
    }
}

const char *token_type_name(TokenType type) {
    switch (type) {
        case TOKEN_INT: return "INT";
        case TOKEN_IF: return "IF";
        case TOKEN_ELSE: return "ELSE";
        case TOKEN_WHILE: return "WHILE";
        case TOKEN_ID: return "ID";
        case TOKEN_NUMBER: return "NUMBER";
        case TOKEN_PLUS: return "PLUS";
        case TOKEN_MINUS: return "MINUS";
        case TOKEN_MUL: return "MUL";
        case TOKEN_DIV: return "DIV";
        case TOKEN_ASSIGN: return "ASSIGN";
        case TOKEN_LT: return "LT";
        case TOKEN_GT: return "GT";
        case TOKEN_LE: return "LE";
        case TOKEN_GE: return "GE";
        case TOKEN_EQ: return "EQ";
        case TOKEN_NE: return "NE";
        case TOKEN_AND: return "AND";
        case TOKEN_OR: return "OR";
        case TOKEN_LPAREN: return "LPAREN";
        case TOKEN_RPAREN: return "RPAREN";
        case TOKEN_LBRACE: return "LBRACE";
        case TOKEN_RBRACE: return "RBRACE";
        case TOKEN_SEMICOLON: return "SEMICOLON";
        case TOKEN_COMMA: return "COMMA";
        case TOKEN_EOF: return "EOF";
        case TOKEN_ERROR: return "ERROR";
        default: return "UNKNOWN";
    }
}
