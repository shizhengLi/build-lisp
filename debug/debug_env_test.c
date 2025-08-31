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
    lval_add(lambda->sexpr.cell[2], lval_sym("x"));
    
    printf("Evaluating lambda...\n");
    Lval *result = eval(e, lambda);
    
    printf("Result type: %d\n", result->type);
    
    printf("Testing lenv_put...\n");
    Lval *sym = lval_sym("test");
    Lval *val = lval_num(42);
    Lval *put_result = lenv_put(e, sym, val);
    
    printf("Cleaning up...\n");
    
    lval_free(put_result);
    lval_free(sym);
    lval_free(val);
    lval_free(result);
    lenv_free(e);
    
    printf("Done.\n");
    return 0;
}