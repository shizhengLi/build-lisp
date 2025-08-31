#include <stdio.h>
#include <stdlib.h>
#include "lval.h"
#include "eval.h"
#include "env.h"

int main() {
    printf("Testing lambda closure...\n");
    
    // Create environment
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Define a variable: (def {x} 10)
    Lval *def_expr = lval_sexpr();
    lval_add(def_expr, lval_sym("def"));
    lval_add(def_expr, lval_sym("x"));
    lval_add(def_expr, lval_num(10));
    Lval *def_result = eval(e, def_expr);
    lval_free(def_result);
    
    // Create a lambda that captures x: (\ {y} {+ x y})
    Lval *lambda = lval_sexpr();
    lval_add(lambda, lval_sym("\\"));
    lval_add(lambda, lval_sexpr()); // parameters
    lval_add(lambda->sexpr.cell[1], lval_sym("y"));
    lval_add(lambda, lval_sexpr()); // body
    lval_add(lambda->sexpr.cell[2], lval_sym("+"));
    lval_add(lambda->sexpr.cell[2], lval_sym("x"));
    lval_add(lambda->sexpr.cell[2], lval_sym("y"));
    
    printf("Evaluating lambda...\n");
    Lval *result = eval(e, lambda);
    
    if (result->type == 5) { // LVAL_LAMBDA
        printf("Lambda created successfully!\n");
        
        // Test function call: ((\ {y} {+ x y}) 5)
        Lval *call = lval_sexpr();
        lval_add(call, result); // lambda function
        lval_add(call, lval_num(5)); // argument
        
        printf("Calling lambda...\n");
        Lval *call_result = eval(e, call);
        
        printf("Call result type: %d\n", call_result->type);
        if (call_result->type == 0) { // LVAL_NUM
            printf("Call result value: %ld\n", call_result->num);
            if (call_result->num == 15) {
                printf("Closure test PASSED!\n");
            } else {
                printf("Closure test FAILED! Expected 15, got %ld\n", call_result->num);
            }
        }
        
        lval_free(call_result);
    } else {
        printf("Lambda creation failed, got type: %d\n", result->type);
    }
    
    lenv_free(e);
    
    printf("Test completed successfully!\n");
    return 0;
}