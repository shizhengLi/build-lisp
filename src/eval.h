#ifndef EVAL_H
#define EVAL_H

#include "lval.h"
#include "env.h"

Lval *eval(Lenv *e, Lval *v);
Lval *builtin_head(Lval *a);
Lval *builtin_tail(Lval *a);
Lval *builtin_list(Lval *a);
Lval *builtin_cons(Lval *a);
Lval *builtin_join(Lval *a);
Lval *builtin_def(Lenv *e, Lval *a);

#endif