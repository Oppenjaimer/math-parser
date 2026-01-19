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

    Lexer lexer;
    lexer_reset(&lexer, text);
    lexer_print(&lexer);

    return 0;
}