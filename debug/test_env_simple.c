#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lval.h"
#include "../src/eval.h"
#include "../src/env.h"

int main() {
    printf("Testing environment functionality...\n");
    
    // Create environment
    Lenv *env = lenv_new();
    
    // Test 1: Define a variable
    printf("Test 1: Define variable x = 42\n");
    Lval *def_expr = lval_sexpr();
    lval_add(def_expr, lval_sym("def"));
    lval_add(def_expr, lval_sym("x"));
    lval_add(def_expr, lval_num(42));
    
    Lval *result = eval(env, def_expr);
    if (result->type == LVAL_NUM && result->num == 42) {
        printf("✓ Variable definition works\n");
    } else {
        printf("✗ Variable definition failed: ");
        if (result->type == LVAL_ERR) {
            printf("Error: %s\n", result->err);
        } else {
            printf("Got type %d, value %ld\n", result->type, result->num);
        }
    }
    lval_free(result);
    
    // Test 2: Look up variable
    printf("Test 2: Look up variable x\n");
    Lval *lookup_expr = lval_sym("x");
    result = eval(env, lookup_expr);
    if (result->type == LVAL_NUM && result->num == 42) {
        printf("✓ Variable lookup works\n");
    } else {
        printf("✗ Variable lookup failed\n");
    }
    lval_free(result);
    
    // Test 3: Variable in expression
    printf("Test 3: Use variable in expression (+ x 8)\n");
    Lval *expr = lval_sexpr();
    lval_add(expr, lval_sym("+"));
    lval_add(expr, lval_sym("x"));
    lval_add(expr, lval_num(8));
    
    result = eval(env, expr);
    if (result->type == LVAL_NUM && result->num == 50) {
        printf("✓ Variable in expression works\n");
    } else {
        printf("✗ Variable in expression failed: ");
        if (result->type == LVAL_ERR) {
            printf("Error: %s\n", result->err);
        } else {
            printf("Got type %d, value %ld\n", result->type, result->num);
        }
    }
    lval_free(result);
    
    // Test 4: Undefined variable
    printf("Test 4: Look up undefined variable\n");
    Lval *undef_expr = lval_sym("undefined");
    result = eval(env, undef_expr);
    if (result->type == LVAL_ERR) {
        printf("✓ Undefined variable handling works\n");
    } else {
        printf("✗ Undefined variable handling failed\n");
    }
    lval_free(result);
    
    lenv_free(env);
    printf("Environment tests completed.\n");
    return 0;
}