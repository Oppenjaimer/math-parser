#ifndef LEXER_H
#define LEXER_H

typedef enum {
    // Basic types
    TOK_NUMBER,
    TOK_IDENTIFIER,

    // Arithmetic operations
    TOK_PLUS, TOK_MINUS, TOK_STAR,
    TOK_SLASH, TOK_CARET, TOK_EQUAL,
    TOK_BANG, TOK_LPAREN, TOK_RPAREN,

    // Other
    TOK_ERROR,
    TOK_EOF,
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    int length;
} Token;

typedef struct {
    const char *current;
} Lexer;

void lexer_reset(Lexer *lexer, const char *text);
Token lexer_next(Lexer *lexer);
int lexer_print(Lexer *lexer);

#endif