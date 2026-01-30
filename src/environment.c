#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "environment.h"

#define SYMBOL_ARENA_CAPACITY (1024 * 2)

static Symbol *get_symbol(SymbolTable *table, const char *name, int length) {
    for (int i = 0; i < table->count; i++) {
        if (table->symbols[i].length == length && strncmp(table->symbols[i].name, name, length) == 0)
            return &table->symbols[i];
    }

    return NULL;
}

static void set_symbol(SymbolTable *table, const char *name, int length, double value) {
    Symbol *existing = get_symbol(table, name, length);
    if (existing) {
        existing->value = value;
        return;
    }

    if (table->count >= MAX_SYMBOLS) fprintf(stderr, "Error: Symbol table full\n");

    char *persistent_name = arena_alloc(table->arena, length + 1);
    if (persistent_name == NULL) {
        fprintf(stderr, "Error: Unable to allocate symbol\n");
        return;
    }

    memcpy(persistent_name, name, length);
    persistent_name[length] = '\0';
    table->symbols[table->count++] = (Symbol){persistent_name, length, value};
}

SymbolTable symbol_table_init() {
    SymbolTable table;
    table.count = 0;
    table.arena = arena_init(SYMBOL_ARENA_CAPACITY);

    if (table.arena == NULL) {
        fprintf(stderr, "Error: Unable to initialize symbol arena\n");
        exit(EXIT_FAILURE);
    }

    set_symbol(&table, "e", 1, 2.7182818284590452354);
    set_symbol(&table, "pi", 2, 3.14159265358979323846);

    return table;
}

void symbol_table_free(SymbolTable *symbol_table) {
    arena_free(symbol_table->arena);
}

void symbol_table_print(SymbolTable *symbol_table) {
    for (int i = 0; i < symbol_table->count; i++) {
        printf("%.*s = %lf\n", symbol_table->symbols[i].length, symbol_table->symbols[i].name, symbol_table->symbols[i].value);
    }
}

double env_evaluate(Node *node, SymbolTable *symbol_table) {
    switch (node->type) {
        case NODE_NUMBER:
            return node->as.number;

        case NODE_IDENTIFIER: {
            Symbol *symbol = get_symbol(symbol_table, node->as.identifier.name, node->as.identifier.length);
            if (symbol) return symbol->value;

            fprintf(stderr, "Error: Undefined variable '%.*s'\n", node->as.identifier.length, node->as.identifier.name);
            return NAN;
        }

        case NODE_UNARY: {
            double right = env_evaluate(node->as.unary.right, symbol_table);
            switch (node->as.unary.op.type) {
                case TOK_PLUS:  return right;
                case TOK_MINUS: return -right;
                case TOK_BANG:  return tgamma(right + 1);
                default:        return 0.0;
            }
        }

        case NODE_BINARY: {
            if (node->as.binary.op.type == TOK_EQUAL) {
                double value = env_evaluate(node->as.binary.right, symbol_table);
                set_symbol(symbol_table, node->as.binary.left->as.identifier.name, node->as.binary.left->as.identifier.length, value);

                return value;
            }

            double left = env_evaluate(node->as.binary.left, symbol_table);
            double right = env_evaluate(node->as.binary.right, symbol_table);

            switch (node->as.binary.op.type) {
                case TOK_PLUS:  return left + right;
                case TOK_MINUS: return left - right;
                case TOK_STAR:  return left * right;
                case TOK_SLASH:
                    if (right == 0.0) {
                        fprintf(stderr, "Error: Division by zero\n");
                        return NAN;
                    }
                    return left / right;
                case TOK_CARET: return pow(left, right);
                default: return 0.0;
            }
        }

        case NODE_CALL: {
            double argument = env_evaluate(node->as.call.argument, symbol_table);
            const char *name = node->as.call.function->as.identifier.name;
            int length = node->as.call.function->as.identifier.length;

            if (strncmp(name, "sin", length) == 0)     return sin(argument);
            if (strncmp(name, "cos", length) == 0)     return cos(argument);
            if (strncmp(name, "tan", length) == 0)     return tan(argument);
            if (strncmp(name, "arcsin", length) == 0)  return asin(argument);
            if (strncmp(name, "arccos", length) == 0)  return acos(argument);
            if (strncmp(name, "arctan", length) == 0)  return atan(argument);
            if (strncmp(name, "sinh", length) == 0)    return sinh(argument);
            if (strncmp(name, "cosh", length) == 0)    return cosh(argument);
            if (strncmp(name, "tanh", length) == 0)    return tanh(argument);
            if (strncmp(name, "arcsinh", length) == 0) return asinh(argument);
            if (strncmp(name, "arccosh", length) == 0) return acosh(argument);
            if (strncmp(name, "arctanh", length) == 0) return atanh(argument);
            if (strncmp(name, "abs", length) == 0)     return fabs(argument);
            if (strncmp(name, "sqrt", length) == 0)    return sqrt(argument);
            if (strncmp(name, "ln", length) == 0)      return log(argument);
            if (strncmp(name, "log", length) == 0)     return log10(argument);
            if (strncmp(name, "exp", length) == 0)     return exp(argument);

            return 0.0;
        }

        default:
            return 0.0;
    }
}