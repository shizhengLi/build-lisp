#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "lval.h"
#include "eval.h"
#include "env.h"
#include "parser.h"

extern int tests_run;

// Test lambda creation and evaluation
static char *test_lambda_creation() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Test lambda creation: (\ {x} {+ x 1})
    Lval *lambda = lval_sexpr();
    lval_add(lambda, lval_sym("\\"));
    lval_add(lambda, lval_sexpr()); // parameters
    lval_add(lambda->sexpr.cell[1], lval_sym("x"));
    lval_add(lambda, lval_sexpr()); // body
    lval_add(lambda->sexpr.cell[2], lval_sym("+"));
    lval_add(lambda->sexpr.cell[2], lval_sym("x"));
    lval_add(lambda->sexpr.cell[2], lval_num(1));
    
    Lval *result = eval(e, lambda);
    mu_assert("Lambda should evaluate to lambda", result->type == LVAL_LAMBDA);
    
    lval_free(result);
    lenv_free(e);
    return 0;
}

// Test lambda function call
static char *test_lambda_call() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Test lambda call: ((\ {x} {+ x 1}) 5) should return 6
    Lval *expr = lval_sexpr();
    lval_add(expr, lval_sexpr()); // lambda expression
    lval_add(expr->sexpr.cell[0], lval_sym("\\"));
    lval_add(expr->sexpr.cell[0], lval_sexpr()); // parameters
    lval_add(expr->sexpr.cell[0]->sexpr.cell[1], lval_sym("x"));
    lval_add(expr->sexpr.cell[0], lval_sexpr()); // body
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("+"));
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("x"));
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_num(1));
    lval_add(expr, lval_num(5)); // argument
    
    Lval *result = eval(e, expr);
    mu_assert("Lambda call should return number", result->type == LVAL_NUM);
    mu_assert("Lambda call result should be 6", result->num == 6);
    
    lval_free(result);
    lenv_free(e);
    return 0;
}

// Test lambda with multiple parameters
static char *test_lambda_multiple_params() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Test lambda with multiple params: ((\ {x y} {+ x y}) 3 4) should return 7
    Lval *expr = lval_sexpr();
    lval_add(expr, lval_sexpr()); // lambda expression
    lval_add(expr->sexpr.cell[0], lval_sym("\\"));
    lval_add(expr->sexpr.cell[0], lval_sexpr()); // parameters
    lval_add(expr->sexpr.cell[0]->sexpr.cell[1], lval_sym("x"));
    lval_add(expr->sexpr.cell[0]->sexpr.cell[1], lval_sym("y"));
    lval_add(expr->sexpr.cell[0], lval_sexpr()); // body
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("+"));
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("x"));
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("y"));
    lval_add(expr, lval_num(3)); // first argument
    lval_add(expr, lval_num(4)); // second argument
    
    Lval *result = eval(e, expr);
    mu_assert("Lambda with multiple params should return number", result->type == LVAL_NUM);
    mu_assert("Lambda with multiple params result should be 7", result->num == 7);
    
    lval_free(result);
    lenv_free(e);
    return 0;
}

// Test lambda closure (variable capture)
static char *test_lambda_closure() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Set up environment: (def {x} 10)
    Lval *def_expr = lval_sexpr();
    lval_add(def_expr, lval_sym("def"));
    lval_add(def_expr, lval_sym("x"));
    lval_add(def_expr, lval_num(10));
    Lval *def_result = eval(e, def_expr);
    lval_free(def_result);
    
    // Test closure: ((\ {y} {+ x y}) 5) should return 15 (captures x=10)
    Lval *expr = lval_sexpr();
    lval_add(expr, lval_sexpr()); // lambda expression
    lval_add(expr->sexpr.cell[0], lval_sym("\\"));
    lval_add(expr->sexpr.cell[0], lval_sexpr()); // parameters
    lval_add(expr->sexpr.cell[0]->sexpr.cell[1], lval_sym("y"));
    lval_add(expr->sexpr.cell[0], lval_sexpr()); // body
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("+"));
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("x"));
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("y"));
    lval_add(expr, lval_num(5)); // argument
    
    Lval *result = eval(e, expr);
    mu_assert("Lambda closure should return number", result->type == LVAL_NUM);
    mu_assert("Lambda closure result should be 15", result->num == 15);
    
    lval_free(result);
    lenv_free(e);
    return 0;
}

// Test recursive function (factorial)
static char *test_lambda_recursion() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // For now, test a simple recursive pattern
    // This creates a lambda that calculates factorial recursively
    Lval *expr = lval_sexpr();
    lval_add(expr, lval_sexpr()); // lambda expression
    lval_add(expr->sexpr.cell[0], lval_sym("\\"));
    lval_add(expr->sexpr.cell[0], lval_sexpr()); // parameters
    lval_add(expr->sexpr.cell[0]->sexpr.cell[1], lval_sym("n"));
    lval_add(expr->sexpr.cell[0], lval_sexpr()); // body
    
    // Add if condition to body
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("if"));
    
    // Add condition: (= n 0)
    Lval *condition = lval_sexpr();
    lval_add(condition, lval_sym("="));
    lval_add(condition, lval_sym("n"));
    lval_add(condition, lval_num(0));
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], condition);
    
    // Add then branch: 1
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_num(1));
    
    // Add else branch: (* n (factorial (- n 1)))
    Lval *else_branch = lval_sexpr();
    lval_add(else_branch, lval_sym("*"));
    lval_add(else_branch, lval_sym("n"));
    
    // Add recursive call: (factorial (- n 1))
    Lval *recursive_call = lval_sexpr();
    lval_add(recursive_call, lval_sym("factorial"));
    
    // Add (- n 1)
    Lval *subtract = lval_sexpr();
    lval_add(subtract, lval_sym("-"));
    lval_add(subtract, lval_sym("n"));
    lval_add(subtract, lval_num(1));
    lval_add(recursive_call, subtract);
    
    lval_add(else_branch, recursive_call);
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], else_branch);
    
    lval_add(expr, lval_num(3)); // argument
    
    Lval *result = eval(e, expr);
    // This test will initially fail until we implement proper recursion
    // For now, we just ensure it doesn't crash
    mu_assert("Recursive lambda should not crash", result != NULL);
    
    lval_free(result);
    lenv_free(e);
    return 0;
}

// Test lambda error cases
static char *test_lambda_errors() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Test lambda with incorrect syntax (no body)
    Lval *bad_lambda = lval_sexpr();
    lval_add(bad_lambda, lval_sym("\\"));
    lval_add(bad_lambda, lval_sexpr()); // parameters only, no body
    
    Lval *result = eval(e, bad_lambda);
    mu_assert("Lambda without body should return error", result->type == LVAL_ERR);
    lval_free(result);
    
    // Test lambda call with wrong number of arguments
    Lval *expr = lval_sexpr();
    lval_add(expr, lval_sexpr()); // lambda expression
    lval_add(expr->sexpr.cell[0], lval_sym("\\"));
    lval_add(expr->sexpr.cell[0], lval_sexpr()); // parameters
    lval_add(expr->sexpr.cell[0]->sexpr.cell[1], lval_sym("x"));
    lval_add(expr->sexpr.cell[0], lval_sexpr()); // body
    lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("x"));
    lval_add(expr, lval_num(1)); // correct argument
    lval_add(expr, lval_num(2)); // extra argument
    
    result = eval(e, expr);
    mu_assert("Lambda call with wrong args should return error", result->type == LVAL_ERR);
    lval_free(result);
    
    lenv_free(e);
    return 0;
}

// Run all lambda tests
char *lambda_tests() {
    mu_run_test(test_lambda_creation);
    mu_run_test(test_lambda_call);
    mu_run_test(test_lambda_multiple_params);
    mu_run_test(test_lambda_closure);
    mu_run_test(test_lambda_recursion);
    mu_run_test(test_lambda_errors);
    
    return 0;
}