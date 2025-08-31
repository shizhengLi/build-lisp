#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "eval.h"
#include "lval.h"
#include "env.h"

Lval *eval_sexpr(Lenv *e, Lval *v);

Lval *eval(Lenv *e, Lval *v) {
    if (v->type == LVAL_SYM) {
        return lenv_get(e, v);
    }
    if (v->type == LVAL_SEXPR) {
        return eval_sexpr(e, v);
    }
    return v;
}

Lval *builtin_op(Lenv *e, Lval *a, char *op) {
    (void)e; // Suppress unused parameter warning
    
    // Check if operator is valid
    if (strcmp(op, "+") != 0 && strcmp(op, "-") != 0 && 
        strcmp(op, "*") != 0 && strcmp(op, "/") != 0 && strcmp(op, "%") != 0 &&
        strcmp(op, "=") != 0 && strcmp(op, ">") != 0 && strcmp(op, "<") != 0 &&
        strcmp(op, ">=") != 0 && strcmp(op, "<=") != 0) {
        lval_free(a);
        return lval_err("Invalid operator!");
    }
    
    // Ensure all arguments are numbers
    for (int i = 0; i < a->sexpr.count; i++) {
        if (a->sexpr.cell[i]->type != LVAL_NUM) {
            lval_free(a);
            return lval_err("Cannot operate on non-number!");
        }
    }
    
    Lval *x = lval_pop(a, 0);
    
    // Handle unary minus
    if ((strcmp(op, "-") == 0) && a->sexpr.count == 0) {
        x->num = -x->num;
    }
    
    // While there are still elements remaining
    // Handle comparison operators separately
    if (strcmp(op, "=") == 0 || strcmp(op, ">") == 0 || strcmp(op, "<") == 0 ||
        strcmp(op, ">=") == 0 || strcmp(op, "<=") == 0) {
        
        int result = 1; // Start with true
        
        for (int i = 0; i < a->sexpr.count; i++) {
            Lval *y = a->sexpr.cell[i];
            
            if (strcmp(op, "=") == 0) { 
                if (x->num != y->num) result = 0;
            }
            if (strcmp(op, ">") == 0) { 
                if (x->num <= y->num) result = 0;
            }
            if (strcmp(op, "<") == 0) { 
                if (x->num >= y->num) result = 0;
            }
            if (strcmp(op, ">=") == 0) { 
                if (x->num < y->num) result = 0;
            }
            if (strcmp(op, "<=") == 0) { 
                if (x->num > y->num) result = 0;
            }
        }
        
        lval_free(x);
        lval_free(a);
        return lval_num(result);
    }
    
    // Handle mathematical operators
    while (a->sexpr.count > 0) {
        Lval *y = lval_pop(a, 0);
        
        if (strcmp(op, "+") == 0) { x->num += y->num; }
        if (strcmp(op, "-") == 0) { x->num -= y->num; }
        if (strcmp(op, "*") == 0) { x->num *= y->num; }
        if (strcmp(op, "/") == 0) { 
            if (y->num == 0) {
                lval_free(x);
                lval_free(y);
                x = lval_err("Division by zero!");
                break;
            }
            x->num /= y->num; 
        }
        if (strcmp(op, "%") == 0) { 
            if (y->num == 0) {
                lval_free(x);
                lval_free(y);
                x = lval_err("Modulo by zero!");
                break;
            }
            x->num %= y->num; 
        }
        
        lval_free(y);
    }
    
    lval_free(a);
    return x;
}

Lval *builtin_head(Lval *a) {
    if (a->sexpr.count != 1) {
        lval_free(a);
        return lval_err("Function 'head' passed too many arguments!");
    }
    
    Lval *list = lval_pop(a, 0);
    if (list->type != LVAL_SEXPR) {
        lval_free(list);
        lval_free(a);
        return lval_err("Function 'head' passed incorrect type!");
    }
    
    if (list->sexpr.count == 0) {
        lval_free(list);
        lval_free(a);
        return lval_err("Function 'head' passed empty list!");
    }
    
    Lval *result = lval_pop(list, 0);
    lval_free(list);
    lval_free(a);
    return result;
}

Lval *builtin_tail(Lval *a) {
    if (a->sexpr.count != 1) {
        lval_free(a);
        return lval_err("Function 'tail' passed too many arguments!");
    }
    
    Lval *list = lval_pop(a, 0);
    if (list->type != LVAL_SEXPR) {
        lval_free(list);
        lval_free(a);
        return lval_err("Function 'tail' passed incorrect type!");
    }
    
    if (list->sexpr.count == 0) {
        lval_free(list);
        lval_free(a);
        return lval_err("Function 'tail' passed empty list!");
    }
    
    lval_free(lval_pop(list, 0));
    Lval *result = list;
    lval_free(a);
    return result;
}

Lval *builtin_list(Lval *a) {
    a->type = LVAL_SEXPR;
    return a;
}

Lval *builtin_cons(Lval *a) {
    if (a->sexpr.count != 2) {
        lval_free(a);
        return lval_err("Function 'cons' passed incorrect number of arguments!");
    }
    
    Lval *elem = lval_pop(a, 0);
    Lval *list = lval_pop(a, 0);
    
    if (list->type != LVAL_SEXPR) {
        lval_free(elem);
        lval_free(list);
        lval_free(a);
        return lval_err("Function 'cons' passed incorrect type!");
    }
    
    lval_add_front(list, elem);
    lval_free(a);
    return list;
}

Lval *builtin_join(Lval *a) {
    for (int i = 0; i < a->sexpr.count; i++) {
        if (a->sexpr.cell[i]->type != LVAL_SEXPR) {
            lval_free(a);
            return lval_err("Function 'join' passed incorrect type!");
        }
    }
    
    Lval *result = lval_pop(a, 0);
    
    while (a->sexpr.count > 0) {
        Lval *next = lval_pop(a, 0);
        while (next->sexpr.count > 0) {
            lval_add(result, lval_pop(next, 0));
        }
        lval_free(next);
    }
    
    lval_free(a);
    return result;
}

Lval *builtin_if(Lenv *e, Lval *a) {
    // Remove the 'if' symbol
    Lval *if_sym = lval_pop(a, 0);
    lval_free(if_sym);
    
    if (a->sexpr.count < 2 || a->sexpr.count > 3) {
        lval_free(a);
        return lval_err("Function 'if' passed incorrect number of arguments!");
    }
    
    // Evaluate condition
    Lval *cond = eval(e, lval_pop(a, 0));
    if (cond->type == LVAL_ERR) {
        lval_free(a);
        return cond;
    }
    
    Lval *result;
    // Check if condition is truthy (non-zero number or non-empty list)
    int truthy = 0;
    if (cond->type == LVAL_NUM) {
        truthy = (cond->num != 0);
    } else if (cond->type == LVAL_SEXPR) {
        truthy = (cond->sexpr.count > 0);
    } else {
        truthy = 1; // Other types are considered truthy
    }
    
    lval_free(cond);
    
    if (truthy) {
        // Return then branch
        result = eval(e, lval_pop(a, 0));
        lval_free(a);
    } else {
        // Return else branch if it exists, otherwise empty list
        if (a->sexpr.count > 1) {
            lval_free(lval_pop(a, 0)); // Remove then branch
            result = eval(e, lval_pop(a, 0));
            lval_free(a);
        } else {
            lval_free(a);
            result = lval_sexpr(); // Empty list
        }
    }
    
    return result;
}

Lval *builtin_lambda(Lenv *e, Lval *a) {
    // Remove the 'lambda' symbol
    Lval *lambda_sym = lval_pop(a, 0);
    lval_free(lambda_sym);
    
    if (a->sexpr.count != 2) {
        lval_free(a);
        return lval_err("Function 'lambda' passed incorrect number of arguments!");
    }
    
    Lval *formals = lval_pop(a, 0);
    Lval *body = lval_pop(a, 0);
    
    // Check if formals is a list of symbols
    for (int i = 0; i < formals->sexpr.count; i++) {
        if (formals->sexpr.cell[i]->type != LVAL_SYM) {
            lval_free(formals);
            lval_free(body);
            lval_free(a);
            return lval_err("Lambda formals must be symbols!");
        }
    }
    
    // Create lambda with current environment
    Lval *result = lval_lambda(formals, body, e);
    lval_free(a);
    
    return result;
}

Lval *builtin_macro(Lenv *e, Lval *a) {
    // Remove the 'macro' symbol
    Lval *macro_sym = lval_pop(a, 0);
    lval_free(macro_sym);
    
    if (a->sexpr.count != 2) {
        lval_free(a);
        return lval_err("Function 'macro' passed incorrect number of arguments!");
    }
    
    Lval *formals = lval_pop(a, 0);
    Lval *body = lval_pop(a, 0);
    
    // Check if formals is a list of symbols
    for (int i = 0; i < formals->sexpr.count; i++) {
        if (formals->sexpr.cell[i]->type != LVAL_SYM) {
            lval_free(formals);
            lval_free(body);
            lval_free(a);
            return lval_err("Macro formals must be symbols!");
        }
    }
    
    // Create macro with current environment
    Lval *result = lval_macro(formals, body, e);
    lval_free(a);
    
    return result;
}

Lval *lval_call(Lenv *e, Lval *f, Lval *a) {
      
    // If it's a macro, perform macro expansion
    if (f->type == LVAL_MACRO) {
        // Check number of arguments
        if (f->macro.formals->sexpr.count != a->sexpr.count) {
            lval_free(a);
            return lval_err("Macro passed wrong number of arguments!");
        }
        
        // Create new environment with parent set to macro's environment (or current if NULL)
        Lenv *new_env = lenv_new();
        new_env->parent = f->macro.env ? f->macro.env : e;
        
        // Bind arguments (unevaluated) to formal parameters
        for (int i = 0; i < f->macro.formals->sexpr.count; i++) {
            Lval *sym = f->macro.formals->sexpr.cell[i];
            Lval *val = a->sexpr.cell[i]; // Use the unevaluated argument
            lenv_put(new_env, sym, val);
        }
        
        // Evaluate the macro body in the new environment to get expanded code
        Lval *expanded = eval(new_env, f->macro.body);
        printf("DEBUG: Macro expanded to type: %d\n", expanded->type);
        
        // Clean up
        lenv_free(new_env);
        
        // Always evaluate the expanded code for code-generation macros
        Lval *result = eval(e, expanded);
        printf("DEBUG: Final result type: %d, value: %ld\n", result->type, result->num);
        lval_free(expanded);
        
        lval_free(a);
        
        return result;
    }
    
    // If it's a lambda function
    if (f->type == LVAL_LAMBDA) {
        // Check number of arguments
        if (f->lambda.formals->sexpr.count != a->sexpr.count) {
            lval_free(a);
            return lval_err("Function passed wrong number of arguments!");
        }
        
        // Create new environment with parent set to lambda's environment
        Lenv *new_env = lenv_new();
        new_env->parent = f->lambda.env;
        
        // Bind arguments to formal parameters
        for (int i = 0; i < f->lambda.formals->sexpr.count; i++) {
            Lval *sym = f->lambda.formals->sexpr.cell[i];
            Lval *val = a->sexpr.cell[i]; // Use the value directly
            lenv_put(new_env, sym, val);
        }
        
        // Evaluate the body in the new environment
        Lval *result = eval(new_env, f->lambda.body);
        
        // Clean up
        lval_free(a);
        lenv_free(new_env);
        
        return result;
    }
    
    // If it's a builtin function, handle as before
    if (f->type == LVAL_FUN) {
        if (strcmp(f->fun, "head") == 0) {
            return builtin_head(a);
        } else if (strcmp(f->fun, "tail") == 0) {
            return builtin_tail(a);
        } else if (strcmp(f->fun, "list") == 0) {
            return builtin_list(a);
        } else if (strcmp(f->fun, "cons") == 0) {
            return builtin_cons(a);
        } else if (strcmp(f->fun, "join") == 0) {
            return builtin_join(a);
        } else {
            return builtin_op(e, a, f->fun);
        }
    }
    
    // If it's neither lambda nor builtin, return error
    lval_free(a);
    return lval_err("Unknown function type!");
}

Lval *builtin_def(Lenv *e, Lval *a) {
    // Remove the 'def' symbol
    Lval *def_sym = lval_pop(a, 0);
    lval_free(def_sym);
    
    if (a->sexpr.count != 2) {
        lval_free(a);
        return lval_err("Function 'def' passed incorrect number of arguments!");
    }
    
    Lval *sym = lval_pop(a, 0);
    if (sym->type != LVAL_SYM) {
        lval_free(sym);
        lval_free(a);
        return lval_err("Function 'def' passed incorrect type!");
    }
    
    Lval *val = eval(e, lval_pop(a, 0));
    lval_free(a);
    
    lenv_put(e, sym, val);
    lval_free(sym);
    
    return val;
}

Lval *eval_sexpr(Lenv *e, Lval *v) {
    // Check for special forms before evaluating children
    if (v->sexpr.count > 0) {
        Lval *first = v->sexpr.cell[0];
        if (first->type == LVAL_SYM) {
            if (strcmp(first->sym, "def") == 0) {
                return builtin_def(e, v);
            }
            if (strcmp(first->sym, "if") == 0) {
                return builtin_if(e, v);
            }
            if (strcmp(first->sym, "\\") == 0) {
                return builtin_lambda(e, v);
            }
            if (strcmp(first->sym, "macro") == 0) {
                return builtin_macro(e, v);
            }
        }
    }
    
    // Evaluate Children
    for (int i = 0; i < v->sexpr.count; i++) {
        v->sexpr.cell[i] = eval(e, v->sexpr.cell[i]);
    }
    
    // Error Checking
    for (int i = 0; i < v->sexpr.count; i++) {
        if (v->sexpr.cell[i]->type == LVAL_ERR) {
            return lval_take(v, i);
        }
    }
    
    // Empty Expression
    if (v->sexpr.count == 0) {
        return v;
    }
    
    // Single Expression
    if (v->sexpr.count == 1) {
        return lval_take(v, 0);
    }
    
    // Ensure First Element is Function or Symbol
    Lval *f = lval_pop(v, 0);
    if (f->type != LVAL_FUN && f->type != LVAL_SYM && f->type != LVAL_LAMBDA && f->type != LVAL_MACRO) {
        lval_free(f);
        lval_free(v);
        return lval_err("S-expression Does not start with function!");
    }
    
    // If it's a symbol, look up the function
    if (f->type == LVAL_SYM) {
        Lval *func = lenv_get(e, f);
        lval_free(f);
        f = func;
        if (f->type != LVAL_FUN && f->type != LVAL_LAMBDA && f->type != LVAL_MACRO) {
            lval_free(f);
            lval_free(v);
            return lval_err("Symbol does not evaluate to function!");
        }
    }
    
    // Call the function using the unified call mechanism
    Lval *result = lval_call(e, f, v);
    // Don't free f here - lval_call will handle it appropriately
    // Only free f if it's a builtin function (not lambda or macro)
    if (f->type == LVAL_FUN) {
        lval_free(f);
    }
    return result;
}