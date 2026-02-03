#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "environment.h"

#define LINE_SIZE 1024

void help() {
    printf("Available commands:\n");
    printf("  .help  Display this help\n");
    printf("  .tree  Toggle AST printing\n");
    printf("  .list  Show list of variables\n");
    printf("  .exit  Quit REPL\n");
}

void parse(Parser *parser, const char *expr, bool show_tree, SymbolTable *symbol_table) {
    Node *root = parser_parse(parser, expr);
    if (root == NULL) return;

    if (show_tree) {
        printf("AST: ");
        node_print(root);
        printf("\n");
    }

    double result = env_evaluate(root, symbol_table);
    printf("%lf\n", result);
}

int main(int argc, char **argv) {
    if (argc > 2) {
        fprintf(stderr, "Usage: %s [EXPRESSION]\n", argv[0]);
        return EXIT_FAILURE;
    }

    Parser parser = parser_init();
    SymbolTable symbol_table = symbol_table_init();

    if (argc == 2) {
        if (strlen(argv[1]) == 0) return EXIT_SUCCESS;

        parse(&parser, argv[1], false, &symbol_table);
        symbol_table_free(&symbol_table);
        parser_free(&parser);

        return EXIT_SUCCESS;
    }

    char line[LINE_SIZE];
    bool show_tree = false;
    printf("Initializing REPL (type '.help' to see available commands)\n");

    while (1) {
        printf("> ");
        if (!fgets(line, sizeof(line), stdin)) break;

        line[strcspn(line, "\n")] = 0;
        if (strlen(line) == 0) continue;
        if (strcmp(line, ".exit") == 0) break;
        if (strcmp(line, ".tree") == 0) {
            show_tree = !show_tree;
            printf("AST printing: %s\n", show_tree ? "ON" : "OFF");
            continue;
        }
        if (strcmp(line, ".list") == 0) {
            symbol_table_print(&symbol_table);
            continue;
        }
        if (strcmp(line, ".help") == 0) {
            help();
            continue;
        }

        parse(&parser, line, show_tree, &symbol_table);
        arena_clear(parser.arena);
    }

    symbol_table_free(&symbol_table);
    parser_free(&parser);
    return EXIT_SUCCESS;
}
