#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

static AstNode *create_number(long value) {
    AstNode *node = malloc(sizeof(AstNode));
    if (node == NULL) return NULL;
    
    node->type = AST_NUMBER;
    node->number = value;
    return node;
}

static AstNode *create_symbol(const char *symbol) {
    AstNode *node = malloc(sizeof(AstNode));
    if (node == NULL) return NULL;
    
    node->type = AST_SYMBOL;
    node->symbol = strdup(symbol);
    return node;
}

static AstNode *create_error(const char *message) {
    AstNode *node = malloc(sizeof(AstNode));
    if (node == NULL) return NULL;
    
    node->type = AST_ERROR;
    node->error = strdup(message);
    return node;
}

static int is_number(const char *str) {
    if (*str == '-' || *str == '+') str++;
    if (*str == '\0') return 0;
    
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

static AstNode *parse_number(const char *input, int *pos) {
    char buffer[256];
    int i = 0;
    
    if (input[*pos] == '-' || input[*pos] == '+') {
        buffer[i++] = input[(*pos)++];
    }
    
    while (isdigit(input[*pos])) {
        buffer[i++] = input[(*pos)++];
    }
    
    buffer[i] = '\0';
    long value = atol(buffer);
    return create_number(value);
}

static AstNode *parse_symbol(const char *input, int *pos) {
    char buffer[256];
    int i = 0;
    
    while (input[*pos] && !isspace(input[*pos]) && input[*pos] != '(' && input[*pos] != ')') {
        buffer[i++] = input[(*pos)++];
    }
    
    buffer[i] = '\0';
    return create_symbol(buffer);
}

static AstNode *parse_sexpr(const char *input, int *pos) {
    // Skip opening '('
    (*pos)++;
    
    // Create empty S-expression
    AstNode *node = malloc(sizeof(AstNode));
    if (node == NULL) return NULL;
    
    node->type = AST_SEXPR;
    node->sexpr.count = 0;
    node->sexpr.children = NULL;
    
    while (input[*pos] && input[*pos] != ')') {
        // Skip whitespace
        while (isspace(input[*pos])) (*pos)++;
        
        if (input[*pos] == '\0') {
            ast_free(node);
            return create_error("Unclosed S-expression");
        }
        
        if (input[*pos] == '(') {
            AstNode *child = parse_sexpr(input, pos);
            if (child == NULL) {
                ast_free(node);
                return NULL;
            }
            if (child->type == AST_ERROR) {
                ast_free(node);
                return child;
            }
            
            // Add child to S-expression
            node->sexpr.count++;
            node->sexpr.children = realloc(node->sexpr.children, sizeof(AstNode*) * node->sexpr.count);
            node->sexpr.children[node->sexpr.count - 1] = child;
        } else if (is_number(&input[*pos])) {
            AstNode *child = parse_number(input, pos);
            if (child == NULL) {
                ast_free(node);
                return NULL;
            }
            
            // Add child to S-expression
            node->sexpr.count++;
            node->sexpr.children = realloc(node->sexpr.children, sizeof(AstNode*) * node->sexpr.count);
            node->sexpr.children[node->sexpr.count - 1] = child;
        } else if (isalpha(input[*pos]) || input[*pos] == '+' || input[*pos] == '-' || input[*pos] == '*' || input[*pos] == '/' || input[*pos] == '%') {
            AstNode *child = parse_symbol(input, pos);
            if (child == NULL) {
                ast_free(node);
                return NULL;
            }
            
            // Add child to S-expression
            node->sexpr.count++;
            node->sexpr.children = realloc(node->sexpr.children, sizeof(AstNode*) * node->sexpr.count);
            node->sexpr.children[node->sexpr.count - 1] = child;
        } else {
            ast_free(node);
            return create_error("Invalid S-expression content");
        }
        
        // Skip whitespace after element
        while (isspace(input[*pos])) (*pos)++;
    }
    
    // Skip closing ')'
    if (input[*pos] == ')') {
        (*pos)++;
    }
    
    return node;
}

AstNode *parse_string(const char *input) {
    int pos = 0;
    
    while (isspace(input[pos])) pos++;
    
    if (input[pos] == '\0') {
        return create_error("Empty input");
    }
    
    if (input[pos] == '(') {
        return parse_sexpr(input, &pos);
    }
    
    if (is_number(&input[pos])) {
        return parse_number(input, &pos);
    }
    
    if (isalpha(input[pos]) || input[pos] == '+' || input[pos] == '-' || input[pos] == '*' || input[pos] == '/' || input[pos] == '%') {
        return parse_symbol(input, &pos);
    }
    
    return create_error("Invalid input");
}

void ast_free(AstNode *node) {
    if (node == NULL) return;
    
    switch (node->type) {
        case AST_SYMBOL:
            free(node->symbol);
            break;
        case AST_SEXPR:
            for (int i = 0; i < node->sexpr.count; i++) {
                ast_free(node->sexpr.children[i]);
            }
            free(node->sexpr.children);
            break;
        case AST_ERROR:
            free(node->error);
            break;
        default:
            break;
    }
    free(node);
}