# Iteration 5: Basic Evaluation

## Overview
In this iteration, we implemented the core evaluation engine that brings our Lisp interpreter to life. This enables the execution of Lisp expressions by converting parsed abstract syntax trees into actual computed values.

## Knowledge Points

### 1. Evaluation Theory
- **Symbol evaluation**: Symbols are looked up in the environment to get their bound values
- **S-expression evaluation**: Lists are evaluated by evaluating each element and applying the first element as a function to the rest
- **Self-evaluating forms**: Numbers, strings, and other literals evaluate to themselves

### 2. Environment Integration
- **Environment parameter**: All evaluation functions now take an environment parameter for variable lookup
- **Symbol resolution**: When evaluating symbols, the environment is consulted to find bound values
- **Error propagation**: Errors during evaluation are properly propagated up the call stack

### 3. Function Application
- **Built-in operators**: Mathematical operators (+, -, *, /, %) are implemented as built-in functions
- **Argument validation**: Functions validate their arguments (type checking, argument count)
- **Error handling**: Comprehensive error handling for invalid operations (division by zero, type mismatches)

## Implementation Details

### Core Evaluation Function
```c
Lval *eval(Lenv *e, Lval *v) {
    if (v->type == LVAL_SYM) {
        return lenv_get(e, v);
    }
    if (v->type == LVAL_SEXPR) {
        return eval_sexpr(e, v);
    }
    return v;
}
```

### S-expression Evaluation
```c
Lval *eval_sexpr(Lenv *e, Lval *v) {
    // Evaluate Children
    for (int i = 0; i < v->sexpr.count; i++) {
        v->sexpr.cell[i] = eval(e, v->sexpr.cell[i]);
    }
    
    // Error Checking
    for (int i = 0; i < v->sexpr.count; i++) {
        if (v->sexpr.cell[i]->type == LVAL_ERR) {
            return lval_take(v, i);
        }
    }
    
    // Empty Expression
    if (v->sexpr.count == 0) {
        return v;
    }
    
    // Single Expression
    if (v->sexpr.count == 1) {
        return lval_take(v, 0);
    }
    
    // Ensure First Element is Symbol
    Lval *f = lval_pop(v, 0);
    if (f->type != LVAL_SYM) {
        lval_free(f);
        lval_free(v);
        return lval_err("S-expression Does not start with symbol!");
    }
    
    // Call builtin with operator
    Lval *result = builtin_op(e, v, f->sym);
    lval_free(f);
    return result;
}
```

### Built-in Operator Implementation
```c
Lval *builtin_op(Lenv *e, Lval *a, char *op) {
    // Check if operator is valid
    if (strcmp(op, "+") != 0 && strcmp(op, "-") != 0 && 
        strcmp(op, "*") != 0 && strcmp(op, "/") != 0 && strcmp(op, "%") != 0) {
        lval_free(a);
        return lval_err("Invalid operator!");
    }
    
    // Ensure all arguments are numbers
    for (int i = 0; i < a->sexpr.count; i++) {
        if (a->sexpr.cell[i]->type != LVAL_NUM) {
            lval_free(a);
            return lval_err("Cannot operate on non-number!");
        }
    }
    
    Lval *x = lval_pop(a, 0);
    
    // Handle unary minus
    if ((strcmp(op, "-") == 0) && a->sexpr.count == 0) {
        x->num = -x->num;
    }
    
    // While there are still elements remaining
    while (a->sexpr.count > 0) {
        Lval *y = lval_pop(a, 0);
        
        if (strcmp(op, "+") == 0) { x->num += y->num; }
        if (strcmp(op, "-") == 0) { x->num -= y->num; }
        if (strcmp(op, "*") == 0) { x->num *= y->num; }
        if (strcmp(op, "/") == 0) { 
            if (y->num == 0) {
                lval_free(x);
                lval_free(y);
                x = lval_err("Division by zero!");
                break;
            }
            x->num /= y->num; 
        }
        if (strcmp(op, "%") == 0) { 
            if (y->num == 0) {
                lval_free(x);
                lval_free(y);
                x = lval_err("Modulo by zero!");
                break;
            }
            x->num %= y->num; 
        }
        
        lval_free(y);
    }
    
    lval_free(a);
    return x;
}
```

## Challenges and Solutions

### 1. Environment Integration
**Problem**: The evaluation system needed to work with the environment system for variable lookup, but the initial design didn't account for this.

**Solution**: Modified all evaluation functions to accept an environment parameter and integrated symbol lookup using the `lenv_get` function.

### 2. Error Propagation
**Problem**: Errors during evaluation need to be properly propagated without memory leaks.

**Solution**: Implemented comprehensive error checking at each stage of evaluation and used the `lval_take` function to properly handle memory management when returning errors.

### 3. Argument Validation
**Problem**: Built-in operators need to validate their arguments before performing operations.

**Solution**: Added type checking to ensure all arguments are numbers and operator validation to ensure only supported operations are performed.

### 4. Memory Management
**Problem**: Complex evaluation logic with multiple temporary values creates opportunities for memory leaks.

**Solution**: Careful use of `lval_pop`, `lval_take`, and `lval_free` to ensure all intermediate values are properly cleaned up.

### 5. Unary Operators
**Problem**: Mathematical operators like `-` can be used as unary operators (negation) or binary operators (subtraction).

**Solution**: Added special handling for unary minus when there's only one argument after the operator.

## Test Coverage
Created comprehensive tests covering:
- Basic evaluation of numbers and symbols
- S-expression evaluation with mathematical operations
- All mathematical operators (+, -, *, /, %)
- Error handling for invalid operations
- Division by zero and modulo by zero errors
- Nested expression evaluation
- Argument type validation

## Current Status
The basic evaluation system is now functional and can handle mathematical expressions with proper error handling. The interpreter can evaluate expressions like `(+ 1 2 3)`, `(- 10 5)`, `(* 2 3 4)`, `(/ 100 5)`, and `(% 17 5)`.

## Next Steps
With basic evaluation working, the next iteration will focus on adding list manipulation functions (head, tail, list, cons, join) to extend the interpreter's capabilities beyond mathematical operations.

## Key Files Modified
- `src/eval.c`: Core evaluation engine implementation
- `src/eval.h`: Evaluation function declarations
- `test/test_eval.c`: Comprehensive test suite for evaluation functionality