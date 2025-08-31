#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lval.h"
#include "../src/eval.h"
#include "../src/env.h"

int main() {
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    printf("Testing symbol evaluation\n");
    
    // Test evaluating = symbol
    Lval* sym = lval_sym("=");
    Lval* result = eval(env, sym);
    
    printf("Symbol '=' evaluation result type: %d\n", result->type);
    if (result->type == LVAL_FUN) {
        printf("Function name: %s\n", result->fun);
    } else if (result->type == LVAL_ERR) {
        printf("Error: %s\n", result->err);
    }
    
    lval_free(result);
    lenv_free(env);
    return 0;
}
