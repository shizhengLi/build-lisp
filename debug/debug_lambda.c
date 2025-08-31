#include <stdio.h>
#include <stdlib.h>
#include "lval.h"
#include "eval.h"
#include "env.h"

int main() {
    printf("Creating environment...\n");
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    printf("Creating lambda...\n");
    Lval *lambda = lval_sexpr();
    lval_add(lambda, lval_sym("\\"));
    lval_add(lambda, lval_sexpr()); // parameters
    lval_add(lambda->sexpr.cell[1], lval_sym("x"));
    lval_add(lambda, lval_sexpr()); // body
    lval_add(lambda->sexpr.cell[2], lval_sym("+"));
    lval_add(lambda->sexpr.cell[2], lval_sym("x"));
    lval_add(lambda->sexpr.cell[2], lval_num(1));
    
    printf("Evaluating lambda...\n");
    Lval *result = eval(e, lambda);
    
    printf("Result type: %d\n", result->type);
    
    printf("Creating function call...\n");
    Lval *call = lval_sexpr();
    lval_add(call, result); // lambda function
    lval_add(call, lval_num(5)); // argument
    
    printf("Evaluating function call...\n");
    Lval *call_result = eval(e, call);
    
    printf("Call result type: %d\n", call_result->type);
    if (call_result->type == LVAL_NUM) {
        printf("Call result value: %ld\n", call_result->num);
    }
    
    printf("Cleaning up...\n");
    
    lval_free(call_result);
    lenv_free(e);
    
    printf("Done.\n");
    return 0;
}