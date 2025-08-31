#include <stdio.h>
#include <stdlib.h>
#include "minunit.h"

int tests_run = 0;

// Function declarations
char *macro_tests(void);

// Test runner
int main() {
    printf("=== Running Macro Tests ===\n");
    
    char *result = macro_tests();
    if (result != 0) {
        printf("FAILED: %s\n", result);
        return 1;
    } else {
        printf("ALL TESTS PASSED\n");
        printf("Tests run: %d\n", tests_run);
        return 0;
    }
}