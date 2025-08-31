#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lval.h"
#include "../src/eval.h"
#include "../src/env.h"

int main() {
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    printf("Testing full S-expression evaluation\n");
    fflush(stdout);
    
    // Create (= 1 0)
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym("="));
    lval_add(expr, lval_num(1));
    lval_add(expr, lval_num(0));
    
    printf("Created expression with type %d and %d elements\n", expr->type, expr->sexpr.count);
    
    Lval* result = eval(env, expr);
    
    printf("Final result type: %d\n", result->type);
    if (result->type == LVAL_NUM) {
        printf("Result: %ld\n", result->num);
    } else if (result->type == LVAL_ERR) {
        printf("Error: %s\n", result->err);
    }
    
    lval_free(result);
    lenv_free(env);
    return 0;
}
