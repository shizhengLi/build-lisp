#include <stdio.h>
#include <stdlib.h>
#include "lval.h"
#include "eval.h"
#include "env.h"

int main() {
    printf("Testing lambda creation...\n");
    
    // Create environment
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Create a simple lambda: (\ {x} x)
    Lval *lambda = lval_sexpr();
    lval_add(lambda, lval_sym("\\"));
    lval_add(lambda, lval_sexpr()); // parameters
    lval_add(lambda->sexpr.cell[1], lval_sym("x"));
    lval_add(lambda, lval_sexpr()); // body
    lval_add(lambda->sexpr.cell[2], lval_sym("x"));
    
    printf("Evaluating lambda...\n");
    Lval *result = eval(e, lambda);
    
    if (result->type == 5) { // LVAL_LAMBDA
        printf("Lambda created successfully!\n");
        
        // Test function call: ((\ {x} x) 42)
        Lval *call = lval_sexpr();
        lval_add(call, result); // lambda function
        lval_add(call, lval_num(42)); // argument
        
        printf("Calling lambda...\n");
        Lval *call_result = eval(e, call);
        
        printf("Call result type: %d\n", call_result->type);
        if (call_result->type == 0) { // LVAL_NUM
            printf("Call result value: %ld\n", call_result->num);
        }
        
        lval_free(call_result);
        // Don't free call here - it's already been freed by eval
    } else {
        printf("Lambda creation failed, got type: %d\n", result->type);
    }
    
    // Don't free result here - it's already been freed by eval
    lenv_free(e);
    
    printf("Test completed successfully!\n");
    return 0;
}