#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "../src/parser.h"

char *test_parse_number() {
    AstNode *node = parse_string("123");
    mu_assert("Error: should parse number", node != NULL);
    mu_assert("Error: should be number type", node->type == AST_NUMBER);
    mu_assert("Error: number value should be 123", node->number == 123);
    ast_free(node);
    
    return NULL;
}

char *test_parse_symbol() {
    AstNode *node = parse_string("+");
    mu_assert("Error: should parse symbol", node != NULL);
    mu_assert("Error: should be symbol type", node->type == AST_SYMBOL);
    mu_assert("Error: symbol should be +", strcmp(node->symbol, "+") == 0);
    ast_free(node);
    
    return NULL;
}

char *test_parse_invalid() {
    AstNode *node = parse_string("@@");
    mu_assert("Error: invalid input should return error node", node != NULL);
    mu_assert("Error: should be error type", node->type == AST_ERROR);
    ast_free(node);
    
    return NULL;
}

char *parser_tests() {
    mu_run_test(test_parse_number);
    mu_run_test(test_parse_symbol);
    mu_run_test(test_parse_invalid);
    return NULL;
}