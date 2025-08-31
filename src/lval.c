#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lval.h"

Lval *lval_num(long x) {
    Lval *v = malloc(sizeof(Lval));
    v->type = LVAL_NUM;
    v->num = x;
    return v;
}

Lval *lval_sym(char *s) {
    Lval *v = malloc(sizeof(Lval));
    v->type = LVAL_SYM;
    v->sym = strdup(s);
    return v;
}

Lval *lval_err(char *m) {
    Lval *v = malloc(sizeof(Lval));
    v->type = LVAL_ERR;
    v->err = strdup(m);
    return v;
}

Lval *lval_fun(char *f) {
    Lval *v = malloc(sizeof(Lval));
    v->type = LVAL_FUN;
    v->fun = strdup(f);
    return v;
}

Lval *lval_sexpr(void) {
    Lval *v = malloc(sizeof(Lval));
    v->type = LVAL_SEXPR;
    v->sexpr.count = 0;
    v->sexpr.cell = NULL;
    return v;
}

void lval_free(Lval *v) {
    if (v == NULL) return;
    
    switch (v->type) {
        case LVAL_SYM: free(v->sym); break;
        case LVAL_ERR: free(v->err); break;
        case LVAL_FUN: free(v->fun); break;
        case LVAL_SEXPR:
            for (int i = 0; i < v->sexpr.count; i++) {
                lval_free(v->sexpr.cell[i]);
            }
            free(v->sexpr.cell);
            break;
        default: break;
    }
    free(v);
}

Lval *lval_add(Lval *v, Lval *x) {
    v->sexpr.count++;
    v->sexpr.cell = realloc(v->sexpr.cell, sizeof(Lval*) * v->sexpr.count);
    v->sexpr.cell[v->sexpr.count - 1] = x;
    return v;
}

Lval *lval_add_front(Lval *v, Lval *x) {
    v->sexpr.count++;
    v->sexpr.cell = realloc(v->sexpr.cell, sizeof(Lval*) * v->sexpr.count);
    memmove(&v->sexpr.cell[1], &v->sexpr.cell[0], sizeof(Lval*) * (v->sexpr.count - 1));
    v->sexpr.cell[0] = x;
    return v;
}

Lval *lval_pop(Lval *v, int i) {
    Lval *x = v->sexpr.cell[i];
    
    // Shift memory after the item at "i" over the top
    memmove(&v->sexpr.cell[i], &v->sexpr.cell[i + 1], sizeof(Lval*) * (v->sexpr.count - i - 1));
    
    v->sexpr.count--;
    v->sexpr.cell = realloc(v->sexpr.cell, sizeof(Lval*) * v->sexpr.count);
    return x;
}

Lval *lval_take(Lval *v, int i) {
    Lval *x = lval_pop(v, i);
    lval_free(v);
    return x;
}

Lval *lval_copy(Lval *v) {
    Lval *x = malloc(sizeof(Lval));
    x->type = v->type;
    
    switch (v->type) {
        case LVAL_NUM:
            x->num = v->num;
            break;
        case LVAL_SYM:
            x->sym = malloc(strlen(v->sym) + 1);
            strcpy(x->sym, v->sym);
            break;
        case LVAL_ERR:
            x->err = malloc(strlen(v->err) + 1);
            strcpy(x->err, v->err);
            break;
        case LVAL_FUN:
            x->fun = malloc(strlen(v->fun) + 1);
            strcpy(x->fun, v->fun);
            break;
        case LVAL_SEXPR:
            x->sexpr.count = v->sexpr.count;
            x->sexpr.cell = malloc(sizeof(Lval*) * x->sexpr.count);
            for (int i = 0; i < x->sexpr.count; i++) {
                x->sexpr.cell[i] = lval_copy(v->sexpr.cell[i]);
            }
            break;
    }
    
    return x;
}

char *lval_to_string(Lval *v) {
    char *result = malloc(1024);
    if (result == NULL) return NULL;
    
    switch (v->type) {
        case LVAL_NUM:
            snprintf(result, 1024, "%ld", v->num);
            break;
        case LVAL_SYM:
            strncpy(result, v->sym, 1023);
            result[1023] = '\0';
            break;
        case LVAL_ERR:
            snprintf(result, 1024, "Error: %s", v->err);
            break;
        case LVAL_FUN:
            snprintf(result, 1024, "<function %s>", v->fun);
            break;
        case LVAL_SEXPR:
            strcpy(result, "(");
            for (int i = 0; i < v->sexpr.count; i++) {
                char *child_str = lval_to_string(v->sexpr.cell[i]);
                strcat(result, child_str);
                free(child_str);
                if (i != v->sexpr.count - 1) {
                    strcat(result, " ");
                }
            }
            strcat(result, ")");
            break;
        default:
            strcpy(result, "Unknown type");
    }
    
    return result;
}