#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "repl.h"
#include "parser.h"
#include "eval.h"
#include "env.h"
#include "lval.h"

static Lval *ast_to_lval(AstNode *node) {
    if (node == NULL) return NULL;
    
    switch (node->type) {
        case AST_NUMBER:
            return lval_num(node->number);
        case AST_SYMBOL:
            return lval_sym(node->symbol);
        case AST_SEXPR: {
            Lval *lval = lval_sexpr();
            for (int i = 0; i < node->sexpr.count; i++) {
                Lval *child = ast_to_lval(node->sexpr.children[i]);
                if (child == NULL) {
                    lval_free(lval);
                    return NULL;
                }
                lval_add(lval, child);
            }
            return lval;
        }
        case AST_ERROR:
            return lval_err(node->error);
        default:
            return lval_err("Unknown AST type");
    }
}

char *read_input(char *input) {
    if (input == NULL || strlen(input) == 0) {
        return NULL;
    }
    
    char *output = malloc(strlen(input) + 1);
    if (output == NULL) {
        return NULL;
    }
    
    strcpy(output, input);
    return output;
}


void start_repl() {
    printf("Lisp REPL v0.2\n");
    printf("Press Ctrl+C to exit\n");
    
    Lenv *env = lenv_new();
    lenv_add_builtins(env);
    
    while (1) {
        printf("lisp> ");
        
        char input[256];
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }
        
        // Remove newline character
        input[strcspn(input, "\n")] = '\0';
        
        if (strlen(input) == 0) {
            continue;
        }
        
        AstNode *node = parse_string(input);
        if (node != NULL) {
            Lval *lval = ast_to_lval(node);
            if (lval != NULL) {
                Lval *result = eval(env, lval);
                char *str = lval_to_string(result);
                printf("%s\n", str);
                free(str);
                lval_free(result);
            } else {
                printf("Conversion error\n");
            }
            ast_free(node);
        } else {
            printf("Parse error\n");
        }
    }
    
    lenv_free(env);
    printf("Goodbye!\n");
}