#ifndef PARSER_H
#define PARSER_H

typedef enum {
    AST_NUMBER,
    AST_SYMBOL,
    AST_SEXPR,
    AST_ERROR
} AstType;

typedef struct AstNode {
    AstType type;
    union {
        long number;
        char *symbol;
        struct {
            struct AstNode **children;
            int count;
        } sexpr;
        char *error;
    };
} AstNode;

AstNode *parse_string(const char *input);
void ast_free(AstNode *node);

#endif