#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "lval.h"
#include "eval.h"
#include "env.h"
#include "parser.h"

char* test_builtin_head() {
    // Test head on a list of numbers
    Lval* list = lval_sexpr();
    lval_add(list, lval_num(1));
    lval_add(list, lval_num(2));
    lval_add(list, lval_num(3));
    
    Lval* args = lval_sexpr();
    lval_add(args, list);
    
    Lval* result = builtin_head(args);
    mu_assert("result should be number", result->type == LVAL_NUM);
    mu_assert("result should be 1", result->num == 1);
    
    lval_free(result);
    // args is already freed by builtin_head
    
    // Test head on empty list
    Lval* empty_list = lval_sexpr();
    Lval* empty_args = lval_sexpr();
    lval_add(empty_args, empty_list);
    
    Lval* empty_result = builtin_head(empty_args);
    mu_assert("empty head should be error", empty_result->type == LVAL_ERR);
    
    lval_free(empty_result);
    // empty_args is already freed by builtin_head
    
    return NULL;
}

char* test_builtin_tail() {
    // Test tail on a list of numbers
    Lval* list = lval_sexpr();
    lval_add(list, lval_num(1));
    lval_add(list, lval_num(2));
    lval_add(list, lval_num(3));
    
    Lval* args = lval_sexpr();
    lval_add(args, list);
    
    Lval* result = builtin_tail(args);
    mu_assert("result should be sexpr", result->type == LVAL_SEXPR);
    mu_assert("result should have 2 elements", result->sexpr.count == 2);
    mu_assert("first element should be 2", result->sexpr.cell[0]->num == 2);
    mu_assert("second element should be 3", result->sexpr.cell[1]->num == 3);
    
    lval_free(result);
    // args is already freed by builtin_tail
    
    return NULL;
}

char* test_builtin_list() {
    // Test list function
    Lval* args = lval_sexpr();
    lval_add(args, lval_num(1));
    lval_add(args, lval_num(2));
    lval_add(args, lval_num(3));
    
    Lval* result = builtin_list(args);
    mu_assert("result should be sexpr", result->type == LVAL_SEXPR);
    mu_assert("result should have 3 elements", result->sexpr.count == 3);
    mu_assert("first element should be 1", result->sexpr.cell[0]->num == 1);
    mu_assert("second element should be 2", result->sexpr.cell[1]->num == 2);
    mu_assert("third element should be 3", result->sexpr.cell[2]->num == 3);
    
    lval_free(result);
    // args is transformed and returned as result by builtin_list
    
    return NULL;
}

char* test_builtin_cons() {
    // Test cons function
    Lval* list = lval_sexpr();
    lval_add(list, lval_num(2));
    lval_add(list, lval_num(3));
    
    Lval* args = lval_sexpr();
    lval_add(args, lval_num(1));
    lval_add(args, list);
    
    Lval* result = builtin_cons(args);
    mu_assert("result should be sexpr", result->type == LVAL_SEXPR);
    mu_assert("result should have 3 elements", result->sexpr.count == 3);
    mu_assert("first element should be 1", result->sexpr.cell[0]->num == 1);
    mu_assert("second element should be 2", result->sexpr.cell[1]->num == 2);
    mu_assert("third element should be 3", result->sexpr.cell[2]->num == 3);
    
    lval_free(result);
    // args is already freed by builtin_cons
    
    return NULL;
}

char* test_builtin_join() {
    // Test join function
    Lval* list1 = lval_sexpr();
    lval_add(list1, lval_num(1));
    lval_add(list1, lval_num(2));
    
    Lval* list2 = lval_sexpr();
    lval_add(list2, lval_num(3));
    lval_add(list2, lval_num(4));
    
    Lval* args = lval_sexpr();
    lval_add(args, list1);
    lval_add(args, list2);
    
    Lval* result = builtin_join(args);
    mu_assert("result should be sexpr", result->type == LVAL_SEXPR);
    mu_assert("result should have 4 elements", result->sexpr.count == 4);
    mu_assert("first element should be 1", result->sexpr.cell[0]->num == 1);
    mu_assert("second element should be 2", result->sexpr.cell[1]->num == 2);
    mu_assert("third element should be 3", result->sexpr.cell[2]->num == 3);
    mu_assert("fourth element should be 4", result->sexpr.cell[3]->num == 4);
    
    lval_free(result);
    // args is already freed by builtin_join
    
    return NULL;
}

char* test_eval_list_functions() {
    // Test evaluating list functions through eval
    Lenv* env = lenv_new();
    lenv_add_builtins(env);
    Lval* expr = lval_sexpr();
    lval_add(expr, lval_sym("list"));
    lval_add(expr, lval_num(1));
    lval_add(expr, lval_num(2));
    lval_add(expr, lval_num(3));
    
    Lval* result = eval(env, expr);
    mu_assert("result should be sexpr", result->type == LVAL_SEXPR);
    mu_assert("result should have 3 elements", result->sexpr.count == 3);
    
    lval_free(result);
    lenv_free(env);
    
    return NULL;
}

char* builtin_tests() {
    mu_run_test(test_builtin_head);
    mu_run_test(test_builtin_tail);
    mu_run_test(test_builtin_list);
    mu_run_test(test_builtin_cons);
    mu_run_test(test_builtin_join);
    mu_run_test(test_eval_list_functions);
    
    return NULL;
}