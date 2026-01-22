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

static Node *expression(Parser *parser, BindingPower right_bp); // Forward declaration
static ParseRule *get_rule(Token token); // Forward declaration

static Node *number(Parser *parser, Node *left) {
    (void)left;
    Token token = parser->previous;

    char buffer[NUMBER_BUFSIZE];
    sprintf(buffer, "%.*s", token.length, token.start);

    Node *node = make_node(parser, NODE_NUMBER);
    if (node == NULL) return NULL;

    node->as.number = strtod(buffer, NULL);
    return node;
}

static Node *identifier(Parser *parser, Node *left) {
    (void)left;
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

static Node *unary(Parser *parser, Node *left) {
    (void)left;
    Token token = parser->previous;

    Node *node = make_node(parser, NODE_UNARY);
    if (node == NULL) return NULL;

    node->as.unary.op = token;
    node->as.unary.right = expression(parser, BP_PREFIX);

    return node;
}

static Node *binary(Parser *parser, Node *left) {
    Token token = parser->previous;

    Node *node = make_node(parser, NODE_BINARY);
    if (node == NULL) return NULL;

    node->as.binary.op = token;
    node->as.binary.left = left;

    BindingPower next_bp = get_rule(token)->left_bp;
    if (token.type == TOK_CARET) {
        // Right-associative
        node->as.binary.right = expression(parser, (BindingPower)((int)next_bp - 1));
    } else {
        // Left-associative
        node->as.binary.right = expression(parser, next_bp);
    }

    return node;
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

static Node *expression(Parser *parser, BindingPower right_bp) {
    Token first_token = consume(parser);
    ParseRule *first_rule = get_rule(first_token);
    if (first_rule == NULL || first_rule->prefix == NULL) return NULL;

    Node *left = first_rule->prefix(parser, NULL);

    while ((int)right_bp < (int)get_rule(peek(parser))->left_bp) {
        Token token = consume(parser);
        ParseRule *rule = get_rule(token);
        if (rule == NULL || rule->infix == NULL) return left;

        left = rule->infix(parser, left);
    }

    return left;
}

void node_print(Node *node) {
    switch (node->type) {
        case NODE_NUMBER:
            printf("%lf", node->as.number);
            break;

        case NODE_IDENTIFIER:
            printf("%.*s", node->as.identifier.length, node->as.identifier.name);
            break;

        case NODE_UNARY:
            printf("(%.*s ", node->as.unary.op.length, node->as.unary.op.start);
            node_print(node->as.unary.right);
            printf(")");
            break;

        case NODE_BINARY:
            printf("(%.*s ", node->as.binary.op.length, node->as.binary.op.start);
            node_print(node->as.binary.left);
            printf(" ");
            node_print(node->as.binary.right);
            printf(")");
            break;

        case NODE_GROUPING:
            printf("Node(grouping)");
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

    return expression(parser, BP_NONE);
}