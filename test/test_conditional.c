#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "lval.h"
#include "eval.h"
#include "env.h"

char *test_builtin_if() {
    // Test basic if with true condition
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    // Create (if (= 1 1) 42 0)
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym("if"));
    
    Lval* condition = lval_sexpr();
    lval_add(condition, lval_sym("="));
    lval_add(condition, lval_num(1));
    lval_add(condition, lval_num(1));
    lval_add(expr, condition);
    
    lval_add(expr, lval_num(42));
    lval_add(expr, lval_num(0));
    
    Lval* result = eval(env, expr);
    mu_assert("if with true condition should return then branch", result->type == LVAL_NUM && result->num == 42);
    lval_free(result);
    
    // Test basic if with false condition
    expr = lval_sexpr();
    lval_add(expr, lval_sym("if"));
    
    condition = lval_sexpr();
    lval_add(condition, lval_sym("="));
    lval_add(condition, lval_num(1));
    lval_add(condition, lval_num(0));
    lval_add(expr, condition);
    
    lval_add(expr, lval_num(42));
    lval_add(expr, lval_num(0));
    
    result = eval(env, expr);
    mu_assert("if with false condition should return else branch", result->type == LVAL_NUM && result->num == 0);
    lval_free(result);
    
    // Test if without else branch (true condition)
    expr = lval_sexpr();
    lval_add(expr, lval_sym("if"));
    
    condition = lval_sexpr();
    lval_add(condition, lval_sym("="));
    lval_add(condition, lval_num(1));
    lval_add(condition, lval_num(1));
    lval_add(expr, condition);
    
    lval_add(expr, lval_num(42));
    
    result = eval(env, expr);
    mu_assert("if with true condition and no else should return then branch", result->type == LVAL_NUM && result->num == 42);
    lval_free(result);
    
    // Test if without else branch (false condition)
    expr = lval_sexpr();
    lval_add(expr, lval_sym("if"));
    
    condition = lval_sexpr();
    lval_add(condition, lval_sym("="));
    lval_add(condition, lval_num(1));
    lval_add(condition, lval_num(0));
    lval_add(expr, condition);
    
    lval_add(expr, lval_num(42));
    
    result = eval(env, expr);
    mu_assert("if with false condition and no else should return empty list", result->type == LVAL_SEXPR && result->sexpr.count == 0);
    lval_free(result);
    
    lenv_free(env);
    return NULL;
}

char *test_builtin_equal() {
    // Test equality comparison
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    // Test (= 1 1)
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym("="));
    lval_add(expr, lval_num(1));
    lval_add(expr, lval_num(1));
    
    Lval* result = eval(env, expr);
    mu_assert("equal numbers should return truthy", result->type == LVAL_NUM && result->num == 1);
    lval_free(result);
    
    // Test (= 1 2)
    expr = lval_sexpr();
    lval_add(expr, lval_sym("="));
    lval_add(expr, lval_num(1));
    lval_add(expr, lval_num(2));
    
    result = eval(env, expr);
    mu_assert("unequal numbers should return falsy", result->type == LVAL_NUM && result->num == 0);
    lval_free(result);
    
    // Test multiple arguments (= 1 1 1)
    expr = lval_sexpr();
    lval_add(expr, lval_sym("="));
    lval_add(expr, lval_num(1));
    lval_add(expr, lval_num(1));
    lval_add(expr, lval_num(1));
    
    result = eval(env, expr);
    mu_assert("equal multiple numbers should return truthy", result->type == LVAL_NUM && result->num == 1);
    lval_free(result);
    
    // Test (= 1 2 1)
    expr = lval_sexpr();
    lval_add(expr, lval_sym("="));
    lval_add(expr, lval_num(1));
    lval_add(expr, lval_num(2));
    lval_add(expr, lval_num(1));
    
    result = eval(env, expr);
    mu_assert("unequal multiple numbers should return falsy", result->type == LVAL_NUM && result->num == 0);
    lval_free(result);
    
    lenv_free(env);
    return NULL;
}

char *test_builtin_ordering() {
    // Test greater than
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    // Test (> 5 3)
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym(">"));
    lval_add(expr, lval_num(5));
    lval_add(expr, lval_num(3));
    
    Lval* result = eval(env, expr);
    mu_assert("greater than true should return truthy", result->type == LVAL_NUM && result->num == 1);
    lval_free(result);
    
    // Test (> 3 5)
    expr = lval_sexpr();
    lval_add(expr, lval_sym(">"));
    lval_add(expr, lval_num(3));
    lval_add(expr, lval_num(5));
    
    result = eval(env, expr);
    mu_assert("greater than false should return falsy", result->type == LVAL_NUM && result->num == 0);
    lval_free(result);
    
    // Test less than
    expr = lval_sexpr();
    lval_add(expr, lval_sym("<"));
    lval_add(expr, lval_num(3));
    lval_add(expr, lval_num(5));
    
    result = eval(env, expr);
    mu_assert("less than true should return truthy", result->type == LVAL_NUM && result->num == 1);
    lval_free(result);
    
    expr = lval_sexpr();
    lval_add(expr, lval_sym("<"));
    lval_add(expr, lval_num(5));
    lval_add(expr, lval_num(3));
    
    result = eval(env, expr);
    mu_assert("less than false should return falsy", result->type == LVAL_NUM && result->num == 0);
    lval_free(result);
    
    lenv_free(env);
    return NULL;
}

char *test_error_conditions() {
    // Test error conditions for conditionals
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    
    // Test if with wrong number of arguments
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym("if"));
    
    Lval* result = eval(env, expr);
    mu_assert("if with no arguments should return error", result->type == LVAL_ERR);
    lval_free(result);
    
    // Test if with only condition
    expr = lval_sexpr();
    lval_add(expr, lval_sym("if"));
    lval_add(expr, lval_num(1));
    
    result = eval(env, expr);
    mu_assert("if with only condition should return error", result->type == LVAL_ERR);
    lval_free(result);
    
    // Test comparison with wrong number of arguments
    expr = lval_sexpr();
    lval_add(expr, lval_sym("="));
    
    result = eval(env, expr);
    mu_assert("= with no arguments should return function", result->type == LVAL_FUN);
    lval_free(result);
    
    expr = lval_sexpr();
    lval_add(expr, lval_sym("="));
    lval_add(expr, lval_num(1));
    
    result = eval(env, expr);
    mu_assert("= with one argument should return the number", result->type == LVAL_NUM && result->num == 1);
    lval_free(result);
    
    lenv_free(env);
    return NULL;
}

char *conditional_tests() {
    mu_run_test(test_builtin_if);
    mu_run_test(test_builtin_equal);
    mu_run_test(test_builtin_ordering);
    mu_run_test(test_error_conditions);
    return NULL;
}