#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "../src/lval.h"

char *test_lval_num() {
    Lval *val = lval_num(123);
    mu_assert("Error: should create number", val != NULL);
    mu_assert("Error: should be number type", val->type == LVAL_NUM);
    mu_assert("Error: number value should be 123", val->num == 123);
    lval_free(val);
    
    return NULL;
}

char *test_lval_sym() {
    Lval *val = lval_sym("+");
    mu_assert("Error: should create symbol", val != NULL);
    mu_assert("Error: should be symbol type", val->type == LVAL_SYM);
    mu_assert("Error: symbol should be +", strcmp(val->sym, "+") == 0);
    lval_free(val);
    
    return NULL;
}

char *test_lval_err() {
    Lval *val = lval_err("Test error");
    mu_assert("Error: should create error", val != NULL);
    mu_assert("Error: should be error type", val->type == LVAL_ERR);
    mu_assert("Error: error message should match", strcmp(val->err, "Test error") == 0);
    lval_free(val);
    
    return NULL;
}

char *test_lval_print() {
    Lval *num = lval_num(42);
    Lval *sym = lval_sym("test");
    Lval *err = lval_err("error message");
    
    // Test printing to string
    char *num_str = lval_to_string(num);
    char *sym_str = lval_to_string(sym);
    char *err_str = lval_to_string(err);
    
    mu_assert("Error: number string should be '42'", strcmp(num_str, "42") == 0);
    mu_assert("Error: symbol string should be 'test'", strcmp(sym_str, "test") == 0);
    mu_assert("Error: error string should contain message", strstr(err_str, "error message") != NULL);
    
    free(num_str);
    free(sym_str);
    free(err_str);
    
    lval_free(num);
    lval_free(sym);
    lval_free(err);
    
    return NULL;
}

char *lval_tests() {
    mu_run_test(test_lval_num);
    mu_run_test(test_lval_sym);
    mu_run_test(test_lval_err);
    mu_run_test(test_lval_print);
    return NULL;
}