#include <stdio.h>
#include <string.h>

#include "lexer.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s EXPRESSION\n", argv[0]);
        return 1;
    }

    const char *text = argv[1];
    if (text == NULL || strlen(text) == 0) {
        fprintf(stderr, "Invalid expression\n");
        return 1;
    }

    Lexer lexer = lexer_new(text);
    Token token = lexer_next(&lexer);

    while (token.type != TOK_EOF) {
        if (token.type == TOK_ERROR) {
            fprintf(stderr, "Error: ");
            token_print(token);
            printf("\n");
            return 1;
        }

        token_print(token);
        printf("\n");
        token = lexer_next(&lexer);
    }

    return 0;
}