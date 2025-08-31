#ifndef LVAL_H
#define LVAL_H

typedef enum {
    LVAL_NUM,
    LVAL_SYM,
    LVAL_SEXPR,
    LVAL_ERR,
    LVAL_FUN
} LvalType;

typedef struct Lval {
    LvalType type;
    union {
        long num;
        char *sym;
        char *err;
        char *fun;
        struct {
            struct Lval **cell;
            int count;
        } sexpr;
    };
} Lval;

Lval *lval_num(long x);
Lval *lval_sym(char *s);
Lval *lval_err(char *m);
Lval *lval_fun(char *f);
Lval *lval_sexpr(void);
Lval *lval_add(Lval *v, Lval *x);
Lval *lval_add_front(Lval *v, Lval *x);
Lval *lval_pop(Lval *v, int i);
Lval *lval_take(Lval *v, int i);
Lval *lval_copy(Lval *v);
void lval_free(Lval *v);
char *lval_to_string(Lval *v);

#endif