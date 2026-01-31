#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include "arena.h"
#include "parser.h"

#define MAX_SYMBOLS 128

typedef struct {
    const char *name;
    int length;
    double value;
} Symbol;

typedef struct {
    Symbol symbols[MAX_SYMBOLS];
    int count;
    Arena *arena;
} SymbolTable;

SymbolTable symbol_table_init();
void symbol_table_free(SymbolTable *symbol_table);
Symbol *symbol_table_get(SymbolTable *table, const char *name, int length);
void symbol_table_set(SymbolTable *table, const char *name, int length, double value);
void symbol_table_print(SymbolTable *symbol_table);
double env_evaluate(Node *node, SymbolTable *symbol_table);

#endif