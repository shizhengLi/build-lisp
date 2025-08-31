#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lval.h"
#include "../src/eval.h"
#include "../src/env.h"

int main() {
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    printf("Testing (= 1 0)\n");
    
    // Test (= 1 0)
    Lval* condition = lval_sexpr();
    lval_add(condition, lval_sym("="));
    lval_add(condition, lval_num(1));
    lval_add(condition, lval_num(0));
    
    Lval* result = eval(env, condition);
    
    if (result->type == LVAL_NUM) {
        printf("Condition result: %ld\n", result->num);
    } else if (result->type == LVAL_ERR) {
        printf("Condition error: %s\n", result->err);
    } else {
        printf("Condition other type: %d\n", result->type);
    }
    
    lval_free(result);
    lenv_free(env);
    return 0;
}
