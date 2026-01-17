#ifndef LEXER_H
#define LEXER_H

typedef enum {
    // Basic types
    TOK_NUMBER,
    TOK_IDENTIFIER,

    // Arithmetic operations
    TOK_PLUS, TOK_MINUS, TOK_STAR,
    TOK_SLASH, TOK_CARET, TOK_EQUAL,
    TOK_LPAREN, TOK_RPAREN,

    // Built-in functions
    TOK_PI, TOK_E,
    TOK_SIN, TOK_COS, TOK_TAN,
    TOK_ASIN, TOK_ACOS, TOK_ATAN,
    TOK_LN, TOK_LOG,
    TOK_ABS, TOK_FAC,

    // Other
    TOK_ERROR,
    TOK_EOF,
} TokenType;

typedef struct {
    TokenType type;
    const char *start;
    int length;
} Token;

void token_print(Token token);

typedef struct {
    const char *current;
} Lexer;

Lexer lexer_new(const char *text);
Token lexer_next(Lexer *lexer);

#endif