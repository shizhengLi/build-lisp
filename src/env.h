#ifndef ENV_H
#define ENV_H

#include "lval.h"

typedef struct Lenv {
    int count;
    char **syms;
    Lval **vals;
    struct Lenv *parent;
} Lenv;

Lenv *lenv_new(void);
void lenv_free(Lenv *e);
Lval *lenv_get(Lenv *e, Lval *k);
void lenv_put(Lenv *e, Lval *k, Lval *v);
void lenv_add_builtins(Lenv *e);

#endif