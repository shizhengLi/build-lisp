#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "lval.h"
#include "eval.h"
#include "env.h"
#include "parser.h"

extern int tests_run;

// Test macro creation
static char *test_macro_creation() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Test macro creation: (macro {x} {+ x 1})
    Lval *macro = lval_sexpr();
    lval_add(macro, lval_sym("macro"));
    lval_add(macro, lval_sexpr()); // parameters
    lval_add(macro->sexpr.cell[1], lval_sym("x"));
    lval_add(macro, lval_sexpr()); // body
    lval_add(macro->sexpr.cell[2], lval_sym("+"));
    lval_add(macro->sexpr.cell[2], lval_sym("x"));
    lval_add(macro->sexpr.cell[2], lval_num(1));
    
    Lval *result = eval(e, macro);
    mu_assert("Macro should evaluate to macro", result->type == LVAL_MACRO);
    mu_assert("Macro should have correct formals count", result->macro.formals->sexpr.count == 1);
    mu_assert("Macro should have correct formals name", strcmp(result->macro.formals->sexpr.cell[0]->sym, "x") == 0);
    
    lval_free(result);
    lenv_free(e);
    
    return 0;
}

// Test macro evaluation with simple arithmetic
static char *test_macro_simple_arithmetic() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Define a macro: (macro inc {x} {+ x 1})
    Lval *macro_def = lval_sexpr();
    lval_add(macro_def, lval_sym("def"));
    lval_add(macro_def, lval_sym("inc"));
    lval_add(macro_def, lval_sexpr());
    lval_add(macro_def->sexpr.cell[2], lval_sym("macro"));
    lval_add(macro_def->sexpr.cell[2], lval_sexpr());
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[1], lval_sym("x"));
    lval_add(macro_def->sexpr.cell[2], lval_sexpr());
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_sym("+"));
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_sym("x"));
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_num(1));
    
    Lval *def_result = eval(e, macro_def);
    lval_free(def_result);
    
    // Use the macro: (inc 5)
    Lval *macro_call = lval_sexpr();
    lval_add(macro_call, lval_sym("inc"));
    lval_add(macro_call, lval_num(5));
    
    Lval *result = eval(e, macro_call);
    mu_assert("Macro call should return number", result->type == LVAL_NUM);
    mu_assert("Macro should compute correctly", result->num == 6);
    
    lval_free(result);
    lenv_free(e);
    
    return 0;
}

// Test macro with multiple parameters
static char *test_macro_multiple_params() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Define a macro: (macro add {a b} {+ a b})
    Lval *macro_def = lval_sexpr();
    lval_add(macro_def, lval_sym("def"));
    lval_add(macro_def, lval_sym("add"));
    lval_add(macro_def, lval_sexpr());
    lval_add(macro_def->sexpr.cell[2], lval_sym("macro"));
    lval_add(macro_def->sexpr.cell[2], lval_sexpr());
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[1], lval_sym("a"));
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[1], lval_sym("b"));
    lval_add(macro_def->sexpr.cell[2], lval_sexpr());
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_sym("+"));
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_sym("a"));
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_sym("b"));
    
    Lval *def_result2 = eval(e, macro_def);
    lval_free(def_result2);
    
    // Use the macro: (add 3 4)
    Lval *macro_call = lval_sexpr();
    lval_add(macro_call, lval_sym("add"));
    lval_add(macro_call, lval_num(3));
    lval_add(macro_call, lval_num(4));
    
    Lval *result = eval(e, macro_call);
    mu_assert("Macro call should return number", result->type == LVAL_NUM);
    mu_assert("Macro should compute correctly", result->num == 7);
    
    lval_free(result);
    lenv_free(e);
    
    return 0;
}

// Test macro with quote - key difference from lambda
static char *test_macro_quote_demonstration() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Define a macro that returns its argument unevaluated: (macro quote-it {x} x)
    Lval *macro_def = lval_sexpr();
    lval_add(macro_def, lval_sym("def"));
    lval_add(macro_def, lval_sym("quote-it"));
    lval_add(macro_def, lval_sexpr());
    lval_add(macro_def->sexpr.cell[2], lval_sym("macro"));
    lval_add(macro_def->sexpr.cell[2], lval_sexpr());
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[1], lval_sym("x"));
    lval_add(macro_def->sexpr.cell[2], lval_sym("x"));
    
    Lval *def_result3 = eval(e, macro_def);
    lval_free(def_result3);
    
    // Use the macro: (quote-it (+ 1 2))
    Lval *macro_call = lval_sexpr();
    lval_add(macro_call, lval_sym("quote-it"));
    lval_add(macro_call, lval_sexpr());
    lval_add(macro_call->sexpr.cell[1], lval_sym("+"));
    lval_add(macro_call->sexpr.cell[1], lval_num(1));
    lval_add(macro_call->sexpr.cell[1], lval_num(2));
    
    Lval *result = eval(e, macro_call);
    mu_assert("Macro should return unevaluated S-expression", result->type == LVAL_SEXPR);
    mu_assert("Should have 3 elements", result->sexpr.count == 3);
    mu_assert("First element should be + symbol", strcmp(result->sexpr.cell[0]->sym, "+") == 0);
    mu_assert("Second element should be number 1", result->sexpr.cell[1]->num == 1);
    mu_assert("Third element should be number 2", result->sexpr.cell[2]->num == 2);
    
    lval_free(result);
    lenv_free(e);
    
    return 0;
}

// Test macro that generates code
static char *test_macro_code_generation() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Define a macro that creates a let expression: (macro let-bind {var val} {def var val})
    Lval *macro_def = lval_sexpr();
    lval_add(macro_def, lval_sym("def"));
    lval_add(macro_def, lval_sym("let-bind"));
    lval_add(macro_def, lval_sexpr());
    lval_add(macro_def->sexpr.cell[2], lval_sym("macro"));
    lval_add(macro_def->sexpr.cell[2], lval_sexpr());
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[1], lval_sym("var"));
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[1], lval_sym("val"));
    lval_add(macro_def->sexpr.cell[2], lval_sexpr());
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_sym("def"));
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_sym("var"));
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_sym("val"));
    
    Lval *def_result4 = eval(e, macro_def);
    lval_free(def_result4);
    
    // Use the macro: (let-bind x 42)
    Lval *macro_call = lval_sexpr();
    lval_add(macro_call, lval_sym("let-bind"));
    lval_add(macro_call, lval_sym("x"));
    lval_add(macro_call, lval_num(42));
    
    Lval *result = eval(e, macro_call);
    mu_assert("Macro should define variable", result->type == LVAL_NUM);
    mu_assert("Variable should have correct value", result->num == 42);
    
    // Check that the variable is now defined
    Lval *x_val = lenv_get(e, lval_sym("x"));
    mu_assert("Variable x should be defined", x_val->type == LVAL_NUM);
    mu_assert("Variable x should have correct value", x_val->num == 42);
    
    lval_free(result);
    lval_free(x_val);
    lenv_free(e);
    
    return 0;
}

// Test macro error handling - wrong number of arguments
static char *test_macro_error_wrong_args() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Define a macro: (macro one-param {x} {+ x 1})
    Lval *macro_def = lval_sexpr();
    lval_add(macro_def, lval_sym("def"));
    lval_add(macro_def, lval_sym("one-param"));
    lval_add(macro_def, lval_sexpr());
    lval_add(macro_def->sexpr.cell[2], lval_sym("macro"));
    lval_add(macro_def->sexpr.cell[2], lval_sexpr());
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[1], lval_sym("x"));
    lval_add(macro_def->sexpr.cell[2], lval_sexpr());
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_sym("+"));
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_sym("x"));
    lval_add(macro_def->sexpr.cell[2]->sexpr.cell[2], lval_num(1));
    
    Lval *def_result5 = eval(e, macro_def);
    lval_free(def_result5);
    
    // Call with wrong number of arguments: (one-param 1 2)
    Lval *macro_call = lval_sexpr();
    lval_add(macro_call, lval_sym("one-param"));
    lval_add(macro_call, lval_num(1));
    lval_add(macro_call, lval_num(2));
    
    Lval *result = eval(e, macro_call);
    mu_assert("Should return error for wrong argument count", result->type == LVAL_ERR);
    mu_assert("Error message should be correct", strstr(result->err, "wrong number of arguments") != NULL);
    
    lval_free(result);
    lenv_free(e);
    
    return 0;
}

// Test macro error handling - invalid macro definition
static char *test_macro_error_invalid_def() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Define macro with wrong number of arguments to macro special form
    Lval *macro_def = lval_sexpr();
    lval_add(macro_def, lval_sym("macro"));
    lval_add(macro_def, lval_sexpr()); // formals
    lval_add(macro_def, lval_sexpr()); // body
    lval_add(macro_def, lval_num(42)); // extra argument
    
    Lval *result = eval(e, macro_def);
    mu_assert("Should return error for invalid macro definition", result->type == LVAL_ERR);
    mu_assert("Error message should be correct", strstr(result->err, "incorrect number of arguments") != NULL);
    
    lval_free(result);
    lenv_free(e);
    
    return 0;
}

// Test macro string representation
static char *test_macro_string_rep() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Create a macro
    Lval *macro = lval_macro(lval_sexpr(), lval_sexpr(), e);
    
    char *str = lval_to_string(macro);
    mu_assert("Macro should stringize correctly", strcmp(str, "<macro>") == 0);
    
    free(str);
    lval_free(macro);
    lenv_free(e);
    
    return 0;
}

// Test macro copying
static char *test_macro_copy() {
    Lenv *e = lenv_new();
    lenv_add_builtins(e);
    
    // Create original macro
    Lval *formals = lval_sexpr();
    lval_add(formals, lval_sym("x"));
    Lval *body = lval_sexpr();
    lval_add(body, lval_sym("x"));
    Lval *original = lval_macro(formals, body, e);
    
    // Copy the macro
    Lval *copy = lval_copy(original);
    
    mu_assert("Copy should be macro type", copy->type == LVAL_MACRO);
    mu_assert("Copy should have same formals count", copy->macro.formals->sexpr.count == original->macro.formals->sexpr.count);
    mu_assert("Copy should have same formals content", strcmp(copy->macro.formals->sexpr.cell[0]->sym, "x") == 0);
    mu_assert("Copy should share environment", copy->macro.env == original->macro.env);
    
    lval_free(original);
    lval_free(copy);
    lenv_free(e);
    
    return 0;
}

// Run all macro tests
char *macro_tests() {
    mu_run_test(test_macro_creation);
    mu_run_test(test_macro_simple_arithmetic);
    mu_run_test(test_macro_multiple_params);
    mu_run_test(test_macro_quote_demonstration);
    mu_run_test(test_macro_code_generation);
    mu_run_test(test_macro_error_wrong_args);
    mu_run_test(test_macro_error_invalid_def);
    mu_run_test(test_macro_string_rep);
    mu_run_test(test_macro_copy);
    return 0;
}