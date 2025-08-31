#include <stdio.h>
#include <stdlib.h>
#include "lval.h"
#include "eval.h"
#include "env.h"

int main() {
    printf("Testing comparison operators...\n");
    
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    // Test = operator with no arguments
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym("="));
    
    printf("Created expression with = operator and no arguments\n");
    printf("Evaluating...\n");
    
    Lval* result = eval(env, expr);
    printf("Evaluation complete\n");
    
    if (result->type == LVAL_ERR) {
        printf("Got expected error: %s\n", result->err);
    } else {
        printf("Got unexpected result: ");
        char* str = lval_to_string(result);
        printf("%s\n", str);
        free(str);
    }
    
    lval_free(result);
    lenv_free(env);
    
    return 0;
}