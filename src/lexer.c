#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>

#include "lexer.h"

static void token_print(FILE *stream, Token token) {
    fprintf(stream, "[Type: %d, Value: %.*s]", token.type, token.length, token.start);
}

static char peek(const Lexer *lexer) {
    return *lexer->current;
}

static char consume(Lexer *lexer) {
    return *lexer->current++;
}

void lexer_reset(Lexer *lexer, const char *text) {
    lexer->current = text;
}

Token lexer_next(Lexer *lexer) {
    while (isspace(peek(lexer))) consume(lexer);

    char c = peek(lexer);
    Token token;
    token.start = lexer->current;

    if (c == '\0') {
        // Do not consume, return EOF indefinitely
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
        case '!': token.type = TOK_BANG;   break;
        case '(': token.type = TOK_LPAREN; break;
        case ')': token.type = TOK_RPAREN; break;
        default:  token.type = TOK_ERROR;  break;
    }

    return token;
}

int lexer_print(Lexer *lexer) {
    Token token = lexer_next(lexer);

    while (token.type != TOK_EOF) {
        if (token.type == TOK_ERROR) {
            fprintf(stderr, "Error: ");
            token_print(stderr, token);
            fprintf(stderr, "\n");
            return 1;
        }

        token_print(stdout, token);
        printf("\n");
        token = lexer_next(lexer);
    }

    return 0;
}