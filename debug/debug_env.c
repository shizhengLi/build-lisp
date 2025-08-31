#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lval.h"
#include "../src/eval.h"
#include "../src/env.h"

int main() {
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    printf("Environment has %d symbols\n", env->count);
    for (int i = 0; i < env->count; i++) {
        printf("Symbol %d: %s\n", i, env->syms[i]);
    }
    
    // Test getting a symbol
    Lval* sym = lval_sym("+");
    Lval* val = lenv_get(env, sym);
    printf("Symbol '+' lookup result type: %d\n", val->type);
    lval_free(sym);
    lval_free(val);
    
    // Test getting = symbol
    sym = lval_sym("=");
    val = lenv_get(env, sym);
    printf("Symbol '=' lookup result type: %d\n", val->type);
    lval_free(sym);
    lval_free(val);
    
    lenv_free(env);
    return 0;
}
