#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "lexer.h"

void token_print(Token token) {
    printf("[Type: %d, Value: %.*s]", token.type, token.length, token.start);
}

Lexer lexer_new(const char *text) {
    return (Lexer){
        .current = text,
    };
}

static char peek(const Lexer *lexer) {
    return *lexer->current;
}

static char consume(Lexer *lexer) {
    return *lexer->current++;
}

static bool is_keyword(Token token, const char *keyword) {
    return (int)strlen(keyword) == token.length && strncmp(token.start, keyword, token.length) == 0;
}

Token lexer_next(Lexer *lexer) {
    while (isspace(peek(lexer))) consume(lexer);

    char c = peek(lexer);
    Token token;
    token.start = lexer->current;

    if (c == '\0') {
        consume(lexer);
        token.type = TOK_EOF;
        token.length = 0;
        return token;
    }

    if (isdigit(c)) {
        while (isdigit(peek(lexer))) consume(lexer);

        if (peek(lexer) == '.') {
            consume(lexer);
            while (isdigit(peek(lexer))) consume(lexer);
        }

        token.type = TOK_NUMBER;
        token.length = (int)(lexer->current - token.start);
        return token;
    }

    if (isalpha(c) || peek(lexer) == '_') {
        while (isalnum(peek(lexer)) || peek(lexer) == '_') consume(lexer);

        token.type = TOK_IDENTIFIER;
        token.length = (int)(lexer->current - token.start);

        if (is_keyword(token, "pi"))        token.type = TOK_PI;
        else if (is_keyword(token, "e"))    token.type = TOK_E;
        else if (is_keyword(token, "sin"))  token.type = TOK_SIN;
        else if (is_keyword(token, "cos"))  token.type = TOK_COS;
        else if (is_keyword(token, "tan"))  token.type = TOK_TAN;
        else if (is_keyword(token, "asin")) token.type = TOK_ASIN;
        else if (is_keyword(token, "acos")) token.type = TOK_ACOS;
        else if (is_keyword(token, "atan")) token.type = TOK_ATAN;
        else if (is_keyword(token, "ln"))   token.type = TOK_LN;
        else if (is_keyword(token, "log"))  token.type = TOK_LOG;
        else if (is_keyword(token, "abs"))  token.type = TOK_ABS;

        return token;
    }

    consume(lexer);
    token.length = 1;

    switch (c) {
        case '+': token.type = TOK_PLUS;   break;
        case '-': token.type = TOK_MINUS;  break;
        case '*': token.type = TOK_STAR;   break;
        case '/': token.type = TOK_SLASH;  break;
        case '^': token.type = TOK_CARET;  break;
        case '=': token.type = TOK_EQUAL;  break;
        case '(': token.type = TOK_LPAREN; break;
        case ')': token.type = TOK_RPAREN; break;
        case '!': token.type = TOK_FAC;    break;
        default:  token.type = TOK_ERROR;  break;
    }

    return token;
}