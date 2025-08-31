#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "../src/lval.h"

char *test_lval_sexpr() {
    Lval *list = lval_sexpr();
    mu_assert("Error: should create empty list", list != NULL);
    mu_assert("Error: should be sexpr type", list->type == LVAL_SEXPR);
    mu_assert("Error: count should be 0", list->sexpr.count == 0);
    
    Lval *num1 = lval_num(1);
    Lval *num2 = lval_num(2);
    
    lval_add(list, num1);
    mu_assert("Error: count should be 1", list->sexpr.count == 1);
    mu_assert("Error: first element should be 1", list->sexpr.cell[0]->num == 1);
    
    lval_add(list, num2);
    mu_assert("Error: count should be 2", list->sexpr.count == 2);
    mu_assert("Error: second element should be 2", list->sexpr.cell[1]->num == 2);
    
    lval_free(list);
    return NULL;
}

char *test_lval_pop() {
    Lval *list = lval_sexpr();
    lval_add(list, lval_num(1));
    lval_add(list, lval_num(2));
    lval_add(list, lval_num(3));
    
    Lval *popped = lval_pop(list, 1);
    mu_assert("Error: popped value should be 2", popped->num == 2);
    mu_assert("Error: list count should be 2", list->sexpr.count == 2);
    mu_assert("Error: first element should still be 1", list->sexpr.cell[0]->num == 1);
    mu_assert("Error: second element should now be 3", list->sexpr.cell[1]->num == 3);
    
    lval_free(popped);
    lval_free(list);
    return NULL;
}

char *test_lval_take() {
    Lval *list = lval_sexpr();
    lval_add(list, lval_num(1));
    lval_add(list, lval_num(2));
    
    Lval *taken = lval_take(list, 0);
    mu_assert("Error: taken value should be 1", taken->num == 1);
    
    lval_free(taken);
    return NULL;
}

char *test_sexpr_print() {
    Lval *list = lval_sexpr();
    lval_add(list, lval_num(1));
    lval_add(list, lval_sym("+"));
    lval_add(list, lval_num(2));
    
    char *str = lval_to_string(list);
    mu_assert("Error: should print as (1 + 2)", strcmp(str, "(1 + 2)") == 0);
    
    free(str);
    lval_free(list);
    return NULL;
}

char *test_nested_sexpr() {
    Lval *outer = lval_sexpr();
    Lval *inner = lval_sexpr();
    
    lval_add(inner, lval_num(1));
    lval_add(inner, lval_num(2));
    lval_add(outer, inner);
    lval_add(outer, lval_num(3));
    
    char *str = lval_to_string(outer);
    mu_assert("Error: should print nested list", strcmp(str, "((1 2) 3)") == 0);
    
    free(str);
    lval_free(outer);
    return NULL;
}

char *sexpr_tests() {
    mu_run_test(test_lval_sexpr);
    mu_run_test(test_lval_pop);
    mu_run_test(test_lval_take);
    mu_run_test(test_sexpr_print);
    mu_run_test(test_nested_sexpr);
    return NULL;
}