#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"

#define LINE_SIZE 1024

void parse(Parser *parser, const char *expr) {
    Node *root = parser_parse(parser, expr);
    if (root != NULL) {
        node_print(root);
        printf("\n");
    }
}

int main(int argc, char **argv) {
    Parser parser = parser_init();

    if (argc > 2) {
        fprintf(stderr, "Usage: %s [EXPRESSION]\n", argv[0]);
        return EXIT_FAILURE;
    }

    if (argc == 2) {
        if (strlen(argv[1]) == 0) return EXIT_SUCCESS;

        parse(&parser, argv[1]);
        parser_free(&parser);

        return EXIT_SUCCESS;
    }

    char line[LINE_SIZE];
    printf("Initializing REPL (type '.exit' to quit)\n");

    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) break;

        line[strcspn(line, "\n")] = 0;
        if (strcmp(line, ".exit") == 0) {
            exit(EXIT_SUCCESS);
        }

        if (strlen(line) == 0) continue;

        parse(&parser, line);
        arena_clear(parser.arena);
    }

    parser_free(&parser);
    return EXIT_SUCCESS;
}