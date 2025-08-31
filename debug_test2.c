#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/lval.h"
#include "../src/eval.h"

int main() {
    printf("Testing invalid operator...\n");
    
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("invalid"));
    lval_add(sexpr, lval_num(1));
    lval_add(sexpr, lval_num(2));
    
    printf("Created sexpr with operator 'invalid'\n");
    
    Lval *result = eval(sexpr);
    printf("Result type: %d\n", result->type);
    printf("LVAL_NUM: %d, LVAL_ERR: %d\n", LVAL_NUM, LVAL_ERR);
    
    if (result->type == LVAL_ERR) {
        printf("SUCCESS: Got error type\n");
        printf("Error message: %s\n", result->err);
    } else {
        printf("FAILURE: Did not get error type\n");
        if (result->type == LVAL_NUM) {
            printf("Got number: %ld\n", result->num);
        }
    }
    
    lval_free(result);
    return 0;
}