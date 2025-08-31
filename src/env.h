#ifndef ENV_H
#define ENV_H

#include "lval.h"

typedef struct Lenv {
    int count;
    char **syms;
    Lval **vals;
} Lenv;

Lenv *lenv_new(void);
void lenv_free(Lenv *e);
Lval *lenv_get(Lenv *e, Lval *k);
Lval *lenv_put(Lenv *e, Lval *k, Lval *v);
void lenv_add_builtins(Lenv *e);

#endif