#include <stdio.h>
#include <stdlib.h>
#include "lval.h"
#include "eval.h"
#include "env.h"

int main() {
    printf("Testing comparison operators...\n");
    
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    // Test = operator with one argument
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym("="));
    lval_add(expr, lval_num(1));
    
    printf("Created expression with = operator and one argument\n");
    printf("Evaluating...\n");
    
    Lval* result = eval(env, expr);
    printf("Evaluation complete\n");
    
    if (result->type == LVAL_ERR) {
        printf("Got error: %s\n", result->err);
    } else if (result->type == LVAL_NUM) {
        printf("Got number: %ld\n", result->num);
    } else {
        printf("Got other type: %d\n", result->type);
    }
    
    lval_free(result);
    lenv_free(env);
    
    return 0;
}