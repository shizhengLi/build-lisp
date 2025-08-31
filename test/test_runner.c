#include <stdio.h>
#include "minunit.h"

int tests_run = 0;

// Function declarations for test suites
char *repl_tests();
char *parser_tests();
char *lval_tests();
char *sexpr_tests();
char *eval_tests();
char *builtin_tests();
char *environment_tests();
char *conditional_tests();

int main() {
    char *result;
    
    printf("Running REPL tests...\n");
    result = repl_tests();
    if (result != 0) {
        printf("%s\n", result);
        return 1;
    }
    
    printf("Running Parser tests...\n");
    result = parser_tests();
    if (result != 0) {
        printf("%s\n", result);
        return 1;
    }
    
    printf("Running Lval tests...\n");
    result = lval_tests();
    if (result != 0) {
        printf("%s\n", result);
        return 1;
    }
    
    printf("Running S-expr tests...\n");
    result = sexpr_tests();
    if (result != 0) {
        printf("%s\n", result);
        return 1;
    }
    
    printf("Running Eval tests...\n");
    result = eval_tests();
    if (result != 0) {
        printf("%s\n", result);
        return 1;
    }
    
    printf("Running Builtin tests...\n");
    result = builtin_tests();
    if (result != 0) {
        printf("%s\n", result);
        return 1;
    }
    
    printf("Running Environment tests...\n");
    result = environment_tests();
    if (result != 0) {
        printf("%s\n", result);
        return 1;
    }
    
    printf("Running Conditional tests...\n");
    result = conditional_tests();
    if (result != 0) {
        printf("%s\n", result);
        return 1;
    }
    
    printf("ALL TESTS PASSED\n");
    printf("Tests run: %d\n", tests_run);
    return 0;
}