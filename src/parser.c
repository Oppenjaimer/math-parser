#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"

#define ARENA_CAPACITY (1024 * 2)
#define NUMBER_BUFSIZE 100

static Node *make_node(Parser *parser, NodeType type) {
    Node *node = arena_alloc(parser->arena, sizeof(Node));
    if (node == NULL) return NULL;

    node->type = type;
    return node;
}

static Node *number(Parser *parser) {
    Token token = parser->previous;

    char buffer[NUMBER_BUFSIZE];
    sprintf(buffer, "%.*s", token.length, token.start);

    Node *node = make_node(parser, NODE_NUMBER);
    if (node == NULL) return NULL;

    node->as.number = strtod(buffer, NULL);
    return node;
}

static Node *identifier(Parser *parser) {
    Token token = parser->previous;

    Node *node = make_node(parser, NODE_IDENTIFIER);
    if (node == NULL) return NULL;

    // TODO: check if math function
    // TODO: check if assigned to variable

    node->as.identifier = (IdentifierData){
        .name = token.start,
        .length = token.length,
    };

    return node;
}

static Node *unary(Parser *parser) {
    return NULL;
}

static Node *binary(Parser *parser) {
    return NULL;
}

ParseRule rules[] = {
    [TOK_NUMBER]     = {number,     NULL,   BP_NONE},
    [TOK_IDENTIFIER] = {identifier, NULL,   BP_NONE},
    [TOK_PLUS]       = {unary,      binary, BP_TERM},
    [TOK_MINUS]      = {unary,      binary, BP_TERM},
    [TOK_STAR]       = {NULL,       binary, BP_FACTOR},
    [TOK_SLASH]      = {NULL,       binary, BP_FACTOR},
    [TOK_CARET]      = {NULL,       binary, BP_POWER},
    [TOK_EQUAL]      = {NULL,       NULL,   BP_NONE}, // TODO
    [TOK_BANG]       = {NULL,       NULL,   BP_NONE}, // TODO
    [TOK_LPAREN]     = {NULL,       NULL,   BP_NONE}, // TODO
    [TOK_RPAREN]     = {NULL,       NULL,   BP_NONE}, // TODO
    [TOK_ERROR]      = {NULL,       NULL,   BP_NONE}, // TODO
    [TOK_EOF]        = {NULL,       NULL,   BP_NONE}, // TODO
};

static ParseRule *get_rule(Token token) {
    if (token.type < 0 || token.type >= sizeof(rules) / sizeof(ParseRule))
        return NULL;

    return &rules[token.type];
}

static Token peek(Parser *parser) {
    return parser->current;
}

static Token consume(Parser *parser) {
    Token current = parser->current;
    parser->previous = current;
    parser->current = lexer_next(&parser->lexer);

    return current;
}

static Node *expression(Parser *parser) {
    Token first_token = consume(parser);
    ParseRule *rule = get_rule(first_token);
    if (rule == NULL || rule->prefix == NULL) return NULL;

    Node *left_node = rule->prefix(parser);

    // TODO: don't return left node, but compute right node

    return left_node;
}

void node_print(Node *node) {
    switch (node->type) {
        case NODE_NUMBER:
            printf("Node(%lf)", node->as.number);
            break;

        case NODE_IDENTIFIER:
            printf("Node(%.*s)", node->as.identifier.length, node->as.identifier.name);
            break;

        // TODO: recursively print all nodes

        default:
            printf("Node()");
            break;
    }
}

Parser parser_init() {
    Lexer lexer = {0};
    Parser parser;
    parser.lexer = lexer;
    parser.arena = arena_init(ARENA_CAPACITY);

    return parser;
}

void parser_free(Parser *parser) {
    arena_free(parser->arena);
}

Node *parser_parse(Parser *parser, const char *expr) {
    lexer_reset(&parser->lexer, expr);
    parser->current = lexer_next(&parser->lexer);
    parser->previous = (Token){ .type = TOK_EOF, .start = 0, .length = 0 }; // Placeholder

    if (parser->current.type == TOK_EOF || parser->current.type == TOK_ERROR)
        return NULL;

    return expression(parser);
}