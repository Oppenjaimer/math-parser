#include <stdio.h>
#include <string.h>

// #include "lexer.h"
#include "parser.h"

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

    // Lexer lexer;
    // lexer_reset(&lexer, text);
    // lexer_print(&lexer);

    Parser parser = parser_init();

    Node *root = parser_parse(&parser, text);
    if (root == NULL) {
        fprintf(stderr, "Parsing error\n");
        return 1;
    }

    node_print(root);
    printf("\n");

    parser_free(&parser);

    return 0;
}