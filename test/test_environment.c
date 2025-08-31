#include <stddef.h>
#include "minunit.h"
#include "lval.h"
#include "eval.h"
#include "env.h"
#include "parser.h"

char* test_variable_definition() {
    // Test defining a variable
    Lenv* env = lenv_new();
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym("def"));
    lval_add(expr, lval_sym("x"));
    lval_add(expr, lval_num(42));
    
    Lval* result = eval(env, expr);
    mu_assert("def should return the defined value", result->type == LVAL_NUM);
    mu_assert("x should be defined as 42", result->num == 42);
    
    lval_free(result);
    lenv_free(env);
    return NULL;
}

char* test_variable_lookup() {
    // Test looking up a defined variable
    Lenv* env = lenv_new();
    Lval* def_expr = lval_sexpr();
    lval_add(def_expr, lval_sym("def"));
    lval_add(def_expr, lval_sym("y"));
    lval_add(def_expr, lval_num(100));
    
    Lval* def_result = eval(env, def_expr);
    lval_free(def_result);
    
    // Now look up the variable
    Lval* lookup_expr = lval_sym("y");
    Lval* lookup_result = eval(env, lookup_expr);
    mu_assert("variable lookup should return number", lookup_result->type == LVAL_NUM);
    mu_assert("y should be 100", lookup_result->num == 100);
    
    lval_free(lookup_result);
    lenv_free(env);
    return NULL;
}

char* test_variable_redefinition() {
    // Test redefining a variable
    Lenv* env = lenv_new();
    Lval* def1_expr = lval_sexpr();
    lval_add(def1_expr, lval_sym("def"));
    lval_add(def1_expr, lval_sym("z"));
    lval_add(def1_expr, lval_num(1));
    
    Lval* def1_result = eval(env, def1_expr);
    lval_free(def1_result);
    
    // Redefine z
    Lval* def2_expr = lval_sexpr();
    lval_add(def2_expr, lval_sym("def"));
    lval_add(def2_expr, lval_sym("z"));
    lval_add(def2_expr, lval_num(2));
    
    Lval* def2_result = eval(env, def2_expr);
    mu_assert("redefinition should return new value", def2_result->type == LVAL_NUM);
    mu_assert("z should now be 2", def2_result->num == 2);
    
    lval_free(def2_result);
    
    // Verify the new value
    Lval* lookup_expr = lval_sym("z");
    Lval* lookup_result = eval(env, lookup_expr);
    mu_assert("z should be 2 after redefinition", lookup_result->num == 2);
    
    lval_free(lookup_result);
    lenv_free(env);
    return NULL;
}

char* test_undefined_variable() {
    // Test looking up an undefined variable
    Lenv* env = lenv_new();
    Lval* expr = lval_sym("undefined_var");
    Lval* result = eval(env, expr);
    mu_assert("undefined variable should return error", result->type == LVAL_ERR);
    
    lval_free(result);
    lenv_free(env);
    return NULL;
}

char* test_variable_in_expression() {
    // Test using variables in mathematical expressions
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    Lval* def_expr = lval_sexpr();
    lval_add(def_expr, lval_sym("def"));
    lval_add(def_expr, lval_sym("a"));
    lval_add(def_expr, lval_num(10));
    
    Lval* def_result = eval(env, def_expr);
    lval_free(def_result);
    
    Lval* def2_expr = lval_sexpr();
    lval_add(def2_expr, lval_sym("def"));
    lval_add(def2_expr, lval_sym("b"));
    lval_add(def2_expr, lval_num(20));
    
    Lval* def2_result = eval(env, def2_expr);
    lval_free(def2_result);
    
    // Use variables in expression
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym("+"));
    lval_add(expr, lval_sym("a"));
    lval_add(expr, lval_sym("b"));
    
    Lval* result = eval(env, expr);
    mu_assert("expression with variables should work", result->type == LVAL_NUM);
    mu_assert("a + b should be 30", result->num == 30);
    
    lval_free(result);
    lenv_free(env);
    return NULL;
}

char* environment_tests() {
    mu_run_test(test_variable_definition);
    mu_run_test(test_variable_lookup);
    mu_run_test(test_variable_redefinition);
    mu_run_test(test_undefined_variable);
    mu_run_test(test_variable_in_expression);
    
    return NULL;
}