#include <stdio.h>
#include <stdlib.h>
#include "../src/lval.h"
#include "../src/env.h"

int main() {
    printf("Testing environment...\n");
    
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    
    printf("Environment created with builtins\n");
    
    // Test symbol lookup
    Lval *sym = lval_sym("+");
    printf("Looking up symbol '+'\n");
    
    Lval *result = lenv_get(env, sym);
    printf("Result type: %d\n", result->type);
    printf("Expected type: %d (LVAL_FUN)\n", LVAL_FUN);
    
    if (result->type == LVAL_FUN) {
        printf("SUCCESS: Symbol '+' found as function\n");
        printf("Function name: %s\n", result->fun);
    } else if (result->type == LVAL_ERR) {
        printf("ERROR: %s\n", result->err);
    } else {
        printf("ERROR: Unexpected type %d\n", result->type);
    }
    
    lval_free(sym);
    lval_free(result);
    lenv_free(env);
    
    return 0;
}