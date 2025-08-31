#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "../src/lval.h"
#include "../src/eval.h"
#include "../src/env.h"

char *test_eval_num() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    Lval *result = eval(env, lval_num(123));
    mu_assert("Error: eval number should return itself", result != NULL);
    mu_assert("Error: should be number type", result->type == LVAL_NUM);
    mu_assert("Error: value should be 123", result->num == 123);
    lval_free(result);
    lenv_free(env);
    
    return NULL;
}

char *test_eval_sym() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    // Test evaluating a symbol that exists (should return function)
    Lval *result = eval(env, lval_sym("+"));
    mu_assert("Error: eval symbol '+' should return function", result != NULL);
    mu_assert("Error: should be function type", result->type == LVAL_FUN);
    lval_free(result);
    
    // Test evaluating a symbol that doesn't exist (should return error)
    Lval *result2 = eval(env, lval_sym("nonexistent"));
    mu_assert("Error: eval nonexistent symbol should return error", result2 != NULL);
    mu_assert("Error: should be error type", result2->type == LVAL_ERR);
    lval_free(result2);
    
    lenv_free(env);
    
    return NULL;
}

char *test_eval_sexpr() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("+"));
    lval_add(sexpr, lval_num(1));
    lval_add(sexpr, lval_num(2));
    
    Lval *result = eval(env, sexpr);
    mu_assert("Error: eval (+ 1 2) should return number", result != NULL);
    mu_assert("Error: should be number type", result->type == LVAL_NUM);
    mu_assert("Error: result should be 3", result->num == 3);
    lval_free(result);
    lenv_free(env);
    
    return NULL;
}

char *test_builtin_add() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("+"));
    lval_add(sexpr, lval_num(10));
    lval_add(sexpr, lval_num(20));
    lval_add(sexpr, lval_num(30));
    
    Lval *result = eval(env, sexpr);
    mu_assert("Error: (+ 10 20 30) should be 60", result->num == 60);
    lval_free(result);
    lenv_free(env);
    
    return NULL;
}

char *test_builtin_sub() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("-"));
    lval_add(sexpr, lval_num(100));
    lval_add(sexpr, lval_num(30));
    lval_add(sexpr, lval_num(20));
    
    Lval *result = eval(env, sexpr);
    mu_assert("Error: (- 100 30 20) should be 50", result->num == 50);
    lval_free(result);
    lenv_free(env);
    
    return NULL;
}

char *test_builtin_mul() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("*"));
    lval_add(sexpr, lval_num(2));
    lval_add(sexpr, lval_num(3));
    lval_add(sexpr, lval_num(4));
    
    Lval *result = eval(env, sexpr);
    mu_assert("Error: (* 2 3 4) should be 24", result->num == 24);
    lval_free(result);
    lenv_free(env);
    
    return NULL;
}

char *test_builtin_div() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("/"));
    lval_add(sexpr, lval_num(100));
    lval_add(sexpr, lval_num(5));
    lval_add(sexpr, lval_num(2));
    
    Lval *result = eval(env, sexpr);
    mu_assert("Error: (/ 100 5 2) should be 10", result->num == 10);
    lval_free(result);
    lenv_free(env);
    
    return NULL;
}

char *test_builtin_mod() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("%"));
    lval_add(sexpr, lval_num(17));
    lval_add(sexpr, lval_num(5));
    
    Lval *result = eval(env, sexpr);
    mu_assert("Error: (% 17 5) should be 2", result->num == 2);
    lval_free(result);
    lenv_free(env);
    
    return NULL;
}

char *test_math_error() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    // Test division by zero
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("/"));
    lval_add(sexpr, lval_num(10));
    lval_add(sexpr, lval_num(0));
    
    Lval *result = eval(env, sexpr);
    mu_assert("Error: division by zero should return error", result != NULL);
    mu_assert("Error: should be error type", result->type == LVAL_ERR);
    lval_free(result);
    
    // Test invalid operator
    Lval *sexpr2 = lval_sexpr();
    lval_add(sexpr2, lval_sym("invalid"));
    lval_add(sexpr2, lval_num(1));
    lval_add(sexpr2, lval_num(2));
    
    Lval *result2 = eval(env, sexpr2);
    mu_assert("Error: invalid operator should return error", result2 != NULL);
    mu_assert("Error: should be error type", result2->type == LVAL_ERR);
    lval_free(result2);
    lenv_free(env);
    
    return NULL;
}

char *test_nested_expressions() {
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    Lval *sexpr = lval_sexpr();
    lval_add(sexpr, lval_sym("+"));
    
    Lval *nested = lval_sexpr();
    lval_add(nested, lval_sym("*"));
    lval_add(nested, lval_num(2));
    lval_add(nested, lval_num(3));
    
    lval_add(sexpr, nested);
    lval_add(sexpr, lval_num(1));
    
    Lval *result = eval(env, sexpr);
    mu_assert("Error: nested expression should work", result != NULL);
    mu_assert("Error: should be number type", result->type == LVAL_NUM);
    mu_assert("Error: (+ (* 2 3) 1) should be 7", result->num == 7);
    lval_free(result);
    lenv_free(env);
    
    return NULL;
}

char *eval_tests() {
    mu_run_test(test_eval_num);
    mu_run_test(test_eval_sym);
    mu_run_test(test_eval_sexpr);
    mu_run_test(test_builtin_add);
    mu_run_test(test_builtin_sub);
    mu_run_test(test_builtin_mul);
    mu_run_test(test_builtin_div);
    mu_run_test(test_builtin_mod);
    mu_run_test(test_math_error);
    mu_run_test(test_nested_expressions);
    return NULL;
}