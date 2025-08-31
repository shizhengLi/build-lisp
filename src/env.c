#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "env.h"
#include "lval.h"

Lenv *lenv_new(void) {
    Lenv *e = malloc(sizeof(Lenv));
    e->count = 0;
    e->syms = NULL;
    e->vals = NULL;
    e->parent = NULL;
    return e;
}

void lenv_free(Lenv *e) {
    if (e == NULL) return;
    
    for (int i = 0; i < e->count; i++) {
        free(e->syms[i]);
        lval_free(e->vals[i]);
    }
    free(e->syms);
    free(e->vals);
    // Don't free parent - it's managed separately
    free(e);
}

Lval *lenv_get(Lenv *e, Lval *k) {
    // Search in current environment
    for (int i = 0; i < e->count; i++) {
        if (strcmp(e->syms[i], k->sym) == 0) {
            Lval *result = lval_copy(e->vals[i]);
            return result;
        }
    }
    
    // If not found and parent exists, search in parent
    if (e->parent) {
        return lenv_get(e->parent, k);
    }
    
    return lval_err("Unbound symbol!");
}

void lenv_put(Lenv *e, Lval *k, Lval *v) {
    
    // Check if variable already exists
    for (int i = 0; i < e->count; i++) {
        if (strcmp(e->syms[i], k->sym) == 0) {
            lval_free(e->vals[i]);
            e->vals[i] = lval_copy(v);
            return;
        }
    }
    
    // If not found, add new variable
    int new_count = e->count + 1;
    e->syms = realloc(e->syms, sizeof(char*) * new_count);
    e->vals = realloc(e->vals, sizeof(Lval*) * new_count);
    e->syms[e->count] = malloc(strlen(k->sym) + 1);
    strcpy(e->syms[e->count], k->sym);
    e->vals[e->count] = lval_copy(v);
    e->count = new_count;
    
}

void lenv_add_builtins(Lenv *e) {
    // Mathematical functions
    char *math_ops[] = {"+", "-", "*", "/", "%"};
    for (int i = 0; i < 5; i++) {
        Lval *sym = lval_sym(math_ops[i]);
        Lval *func = lval_fun(math_ops[i]);
        lenv_put(e, sym, func);
        lval_free(sym);
        lval_free(func);
    }
    
    // Comparison functions
    char *comp_ops[] = {"=", ">", "<", ">=", "<="};
    for (int i = 0; i < 5; i++) {
        Lval *sym = lval_sym(comp_ops[i]);
        Lval *func = lval_fun(comp_ops[i]);
        lenv_put(e, sym, func);
        lval_free(sym);
        lval_free(func);
    }
    
    // List functions
    char *list_funcs[] = {"head", "tail", "list", "cons", "join"};
    for (int i = 0; i < 5; i++) {
        Lval *sym = lval_sym(list_funcs[i]);
        Lval *func = lval_fun(list_funcs[i]);
        lenv_put(e, sym, func);
        lval_free(sym);
        lval_free(func);
    }
}