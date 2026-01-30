#ifndef PARSER_H
#define PARSER_H

#include "arena.h"
#include "lexer.h"

typedef enum {
    NODE_NUMBER,
    NODE_IDENTIFIER,
    NODE_UNARY,
    NODE_BINARY,
    NODE_CALL,
} NodeType;

struct Node; // Forward declaration

typedef struct {
    const char *name;
    int length;
} IdentifierData;

typedef struct {
    Token op;
    struct Node *right;
} UnaryData;

typedef struct {
    Token op;
    struct Node *left;
    struct Node *right;
} BinaryData;

typedef struct {
    struct Node *function;
    struct Node *argument;
} CallData;

typedef struct Node {
    NodeType type;
    union {
        double number;
        IdentifierData identifier;
        UnaryData unary;
        BinaryData binary;
        CallData call;
        struct Node *group;
    } as;
} Node;

typedef struct {
    Lexer lexer;
    Arena *arena;
    Token current;
    Token previous;
} Parser;

typedef enum {
    BP_NONE,
    BP_ASSIGNMENT,
    BP_TERM,
    BP_FACTOR,
    BP_POWER,
    BP_PREFIX,
    BP_POSTFIX,
    BP_CALL,
} BindingPower;

typedef Node *(*ParseFn)(Parser *parser, struct Node *left);

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    BindingPower left_bp;
} ParseRule;

void node_print(Node *node);
Parser parser_init();
void parser_free(Parser *parser);
Node *parser_parse(Parser *parser, const char *expr);

#endif