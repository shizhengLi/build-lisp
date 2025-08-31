#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "minunit.h"
#include "../src/repl.h"

char *test_read_input() {
    char input[256];
    strcpy(input, "hello");
    
    char *output = read_input(input);
    mu_assert("Error: input should be echoed", strcmp(output, "hello") == 0);
    free(output);
    
    return NULL;
}

char *test_empty_input() {
    char input[256];
    strcpy(input, "");
    
    char *output = read_input(input);
    mu_assert("Error: empty input should return NULL", output == NULL);
    
    return NULL;
}

char *repl_tests() {
    mu_run_test(test_read_input);
    mu_run_test(test_empty_input);
    return NULL;
}