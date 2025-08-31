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
    return e;
}

void lenv_free(Lenv *e) {
    for (int i = 0; i < e->count; i++) {
        free(e->syms[i]);
        lval_free(e->vals[i]);
    }
    free(e->syms);
    free(e->vals);
    free(e);
}

Lval *lenv_get(Lenv *e, Lval *k) {
    for (int i = 0; i < e->count; i++) {
        if (strcmp(e->syms[i], k->sym) == 0) {
            return lval_copy(e->vals[i]);
        }
    }
    return lval_err("Unbound symbol!");
}

Lval *lenv_put(Lenv *e, Lval *k, Lval *v) {
    // Check if variable already exists
    for (int i = 0; i < e->count; i++) {
        if (strcmp(e->syms[i], k->sym) == 0) {
            lval_free(e->vals[i]);
            e->vals[i] = lval_copy(v);
            return lval_copy(v);
        }
    }
    
    // If not found, add new variable
    e->count++;
    e->syms = realloc(e->syms, sizeof(char*) * e->count);
    e->vals = realloc(e->vals, sizeof(Lval*) * e->count);
    
    e->syms[e->count - 1] = malloc(strlen(k->sym) + 1);
    strcpy(e->syms[e->count - 1], k->sym);
    e->vals[e->count - 1] = lval_copy(v);
    
    return lval_copy(v);
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