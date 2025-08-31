#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/lval.h"
#include "src/eval.h"
#include "src/env.h"

int main() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    
    // Test basic number evaluation
    printf("Testing eval(lval_num(123))...\n");
    Lval *result = eval(env, lval_num(123));
    printf("Result type: %d\n", result->type);
    printf("Expected type: %d\n", LVAL_NUM);
    if (result->type == LVAL_NUM) {
        printf("SUCCESS: Number evaluates to itself\n");
        printf("Value: %ld\n", result->num);
    } else {
        printf("FAILURE: Number evaluation failed\n");
        if (result->type == LVAL_ERR) {
            printf("Error: %s\n", result->err);
        }
    }
    lval_free(result);
    
    // Test S-expression evaluation
    printf("\nTesting eval of (+ 1 2)...\n");
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("+"));
    lval_add(sexpr, lval_num(1));
    lval_add(sexpr, lval_num(2));
    
    printf("Created S-expression with %d children\n", sexpr->sexpr.count);
    printf("First child type: %d (should be LVAL_SYM=1)\n", sexpr->sexpr.cell[0]->type);
    
    result = eval(env, sexpr);
    printf("Result type: %d\n", result->type);
    printf("Expected type: %d\n", LVAL_NUM);
    if (result->type == LVAL_NUM) {
        printf("SUCCESS: (+ 1 2) evaluates to number\n");
        printf("Value: %ld\n", result->num);
    } else {
        printf("FAILURE: S-expression evaluation failed\n");
        if (result->type == LVAL_ERR) {
            printf("Error: %s\n", result->err);
        }
    }
    lval_free(result);
    
    lenv_free(env);
    return 0;
}