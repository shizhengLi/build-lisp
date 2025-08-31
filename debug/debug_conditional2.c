#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lval.h"
#include "../src/eval.h"
#include "../src/env.h"

int main() {
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    printf("Testing simple = first\n");
    
    // Test (= 1 1)
    Lval* simple = lval_sexpr();
    lval_add(simple, lval_sym("="));
    lval_add(simple, lval_num(1));
    lval_add(simple, lval_num(1));
    
    Lval* result = eval(env, simple);
    
    if (result->type == LVAL_NUM) {
        printf("Simple = result: %ld\n", result->num);
    } else if (result->type == LVAL_ERR) {
        printf("Simple = error: %s\n", result->err);
    } else {
        printf("Simple = other type: %d\n", result->type);
    }
    
    lval_free(result);
    
    printf("Now testing if\n");
    
    // Create (if (= 1 1) 42 0)
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym("if"));
    
    Lval* condition = lval_sexpr();
    lval_add(condition, lval_sym("="));
    lval_add(condition, lval_num(1));
    lval_add(condition, lval_num(1));
    lval_add(expr, condition);
    
    lval_add(expr, lval_num(42));
    lval_add(expr, lval_num(0));
    
    printf("Created expression\n");
    
    result = eval(env, expr);
    printf("Evaluated result\n");
    
    if (result->type == LVAL_NUM) {
        printf("Result: %ld\n", result->num);
    } else if (result->type == LVAL_ERR) {
        printf("Error: %s\n", result->err);
    } else {
        printf("Other type: %d\n", result->type);
    }
    
    lval_free(result);
    lenv_free(env);
    return 0;
}
