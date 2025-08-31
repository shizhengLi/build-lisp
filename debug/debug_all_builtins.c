#include <stdio.h>
#include <stdlib.h>
#include "../src/lval.h"
#include "../src/eval.h"
#include "../src/env.h"

int main() {
    printf("Testing all builtin functions...\n\n");
    
    // Test builtin_head
    printf("1. Testing builtin_head:\n");
    Lval* list = lval_sexpr();
    lval_add(list, lval_num(1));
    lval_add(list, lval_num(2));
    lval_add(list, lval_num(3));
    
    Lval* args = lval_sexpr();
    lval_add(args, list);
    
    Lval* result = builtin_head(args);
    printf("   Result type: %d (expected %d)\n", result->type, LVAL_NUM);
    printf("   Result value: %ld (expected 1)\n", result->num);
    lval_free(result);
    
    // Test builtin_tail
    printf("\n2. Testing builtin_tail:\n");
    list = lval_sexpr();
    lval_add(list, lval_num(1));
    lval_add(list, lval_num(2));
    lval_add(list, lval_num(3));
    
    args = lval_sexpr();
    lval_add(args, list);
    
    result = builtin_tail(args);
    printf("   Result type: %d (expected %d)\n", result->type, LVAL_SEXPR);
    printf("   Result count: %d (expected 2)\n", result->sexpr.count);
    if (result->sexpr.count >= 2) {
        printf("   Elements: %ld, %ld (expected 2, 3)\n", result->sexpr.cell[0]->num, result->sexpr.cell[1]->num);
    }
    lval_free(result);
    
    // Test builtin_list
    printf("\n3. Testing builtin_list:\n");
    args = lval_sexpr();
    lval_add(args, lval_num(1));
    lval_add(args, lval_num(2));
    lval_add(args, lval_num(3));
    
    result = builtin_list(args);
    printf("   Result type: %d (expected %d)\n", result->type, LVAL_SEXPR);
    printf("   Result count: %d (expected 3)\n", result->sexpr.count);
    if (result->sexpr.count >= 3) {
        printf("   Elements: %ld, %ld, %ld (expected 1, 2, 3)\n", 
               result->sexpr.cell[0]->num, result->sexpr.cell[1]->num, result->sexpr.cell[2]->num);
    }
    lval_free(result);
    
    // Test builtin_cons
    printf("\n4. Testing builtin_cons:\n");
    list = lval_sexpr();
    lval_add(list, lval_num(2));
    lval_add(list, lval_num(3));
    
    args = lval_sexpr();
    lval_add(args, lval_num(1));
    lval_add(args, list);
    
    result = builtin_cons(args);
    printf("   Result type: %d (expected %d)\n", result->type, LVAL_SEXPR);
    printf("   Result count: %d (expected 3)\n", result->sexpr.count);
    if (result->sexpr.count >= 3) {
        printf("   Elements: %ld, %ld, %ld (expected 1, 2, 3)\n", 
               result->sexpr.cell[0]->num, result->sexpr.cell[1]->num, result->sexpr.cell[2]->num);
    }
    lval_free(result);
    
    // Test builtin_join
    printf("\n5. Testing builtin_join:\n");
    Lval* list1 = lval_sexpr();
    lval_add(list1, lval_num(1));
    lval_add(list1, lval_num(2));
    
    Lval* list2 = lval_sexpr();
    lval_add(list2, lval_num(3));
    lval_add(list2, lval_num(4));
    
    args = lval_sexpr();
    lval_add(args, list1);
    lval_add(args, list2);
    
    result = builtin_join(args);
    printf("   Result type: %d (expected %d)\n", result->type, LVAL_SEXPR);
    printf("   Result count: %d (expected 4)\n", result->sexpr.count);
    if (result->sexpr.count >= 4) {
        printf("   Elements: %ld, %ld, %ld, %ld (expected 1, 2, 3, 4)\n", 
               result->sexpr.cell[0]->num, result->sexpr.cell[1]->num, 
               result->sexpr.cell[2]->num, result->sexpr.cell[3]->num);
    }
    lval_free(result);
    
    printf("\nAll tests completed!\n");
    return 0;
}