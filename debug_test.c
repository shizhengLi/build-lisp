#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lval.h"
#include "../src/eval.h"

int main() {
    printf("Testing division by zero...\n");
    
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("/"));
    lval_add(sexpr, lval_num(10));
    lval_add(sexpr, lval_num(0));
    
    Lval *result = eval(sexpr);
    printf("Result type: %d\n", result->type);
    printf("Expected error type: %d\n", LVAL_ERR);
    
    if (result->type == LVAL_ERR) {
        printf("SUCCESS: Got error type\n");
        printf("Error message: %s\n", result->err);
    } else {
        printf("FAILURE: Did not get error type\n");
    }
    
    lval_free(result);
    return 0;
}