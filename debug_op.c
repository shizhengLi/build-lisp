#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lval.h"
#include "../src/eval.h"
#include "../src/env.h"

int main() {
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    printf("Testing builtin_op directly\n");
    
    // Create args for (= 1 0)
    Lval* args = lval_sexpr();
    lval_add(args, lval_num(1));
    lval_add(args, lval_num(0));
    
    printf("Created args with count: %d\n", args->sexpr.count);
    
    Lval* result = builtin_op(env, args, "=");
    
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
