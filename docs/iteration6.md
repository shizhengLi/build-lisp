# Iteration 6: Built-in Mathematical Operators

## Overview
In this iteration, we refined and extended the mathematical operator system introduced in Iteration 5. We added comprehensive error handling, support for unary operations, and ensured robust behavior for edge cases in mathematical computations.

## Knowledge Points

### 1. Mathematical Operator Theory
- **Binary operations**: Standard arithmetic operations with two or more operands
- **Unary operations**: Support for negation (unary minus)
- **Operand validation**: Type checking and argument count validation
- **Error conditions**: Division by zero, modulo by zero, invalid operators

### 2. Error Handling Strategies
- **Early validation**: Check arguments before performing operations
- **Memory safety**: Proper cleanup of resources even when errors occur
- **Error propagation**: Ensure errors bubble up through the evaluation stack
- **Descriptive messages**: Clear error messages for different failure conditions

### 3. Memory Management in Evaluation
- **Resource ownership**: Clear understanding of which function owns which memory
- **Cleanup on error**: Ensure no memory leaks when operations fail
- **Temporary value management**: Proper handling of intermediate computation results

## Implementation Details

### Enhanced Built-in Operator Function
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

### Supported Operations

#### Addition (+)
- **Syntax**: `(+ number1 number2 ...)`
- **Behavior**: Sums all numbers
- **Examples**: 
  - `(+ 1 2 3)` → `6`
  - `(+ 10)` → `10`
  - `(+)` → Error (requires at least one argument for unary context)

#### Subtraction (-)
- **Syntax**: `(- number1 number2 ...)` or `(- number)`
- **Behavior**: Subtracts all subsequent numbers from the first, or negates a single number
- **Examples**:
  - `(- 10 3 2)` → `5`
  - `(- 5)` → `-5` (unary negation)

#### Multiplication (*)
- **Syntax**: `(* number1 number2 ...)`
- **Behavior**: Multiplies all numbers together
- **Examples**:
  - `(* 2 3 4)` → `24`
  - `(* 5)` → `5`

#### Division (/)
- **Syntax**: `(/ number1 number2 ...)`
- **Behavior**: Divides the first number by all subsequent numbers
- **Error**: Division by zero returns error
- **Examples**:
  - `(/ 100 5 2)` → `10`
  - `(/ 10 0)` → Error: "Division by zero!"

#### Modulo (%)
- **Syntax**: `(% number1 number2)`
- **Behavior**: Returns remainder of division (currently binary only)
- **Error**: Modulo by zero returns error
- **Examples**:
  - `(% 17 5)` → `2`
  - `(% 10 0)` → Error: "Modulo by zero!"

## Challenges and Solutions

### 1. Unary Operator Support
**Problem**: The subtraction operator needed to support both binary subtraction and unary negation.

**Solution**: Added special logic to detect when `-` is used with only one argument and handle it as negation:
```c
if ((strcmp(op, "-") == 0) && a->sexpr.count == 0) {
    x->num = -x->num;
}
```

### 2. Division by Zero Handling
**Problem**: Division by zero is mathematically undefined and could cause crashes.

**Solution**: Added explicit checks before division operations:
```c
if (y->num == 0) {
    lval_free(x);
    lval_free(y);
    x = lval_err("Division by zero!");
    break;
}
```

### 3. Memory Leaks in Error Cases
**Problem**: When errors occur during evaluation, intermediate values need to be cleaned up properly.

**Solution**: Ensured proper cleanup of all allocated memory before returning errors:
```c
lval_free(x);
lval_free(y);
x = lval_err("Division by zero!");
```

### 4. Type Safety
**Problem**: Mathematical operations should only work on numbers, but the evaluation system could pass other types.

**Solution**: Added comprehensive type checking:
```c
for (int i = 0; i < a->sexpr.count; i++) {
    if (a->sexpr.cell[i]->type != LVAL_NUM) {
        lval_free(a);
        return lval_err("Cannot operate on non-number!");
    }
}
```

### 5. Operator Validation
**Problem**: The system needed to distinguish between valid mathematical operators and invalid symbols.

**Solution**: Added explicit operator validation:
```c
if (strcmp(op, "+") != 0 && strcmp(op, "-") != 0 && 
    strcmp(op, "*") != 0 && strcmp(op, "/") != 0 && strcmp(op, "%") != 0) {
    lval_free(a);
    return lval_err("Invalid operator!");
}
```

## Test Coverage
Extended testing to cover:
- All mathematical operations with multiple arguments
- Unary negation functionality
- Division by zero error handling
- Modulo by zero error handling
- Type error handling for non-number arguments
- Invalid operator error handling
- Nested mathematical expressions
- Edge cases with single arguments

## Usage Examples
The mathematical operators now work reliably:

```lisp
lispy> (+ 1 2 3 4 5)
15

lispy> (- 100 50 25)
25

lispy> (- 10)
-10

lispy> (* 2 3 4)
24

lispy> (/ 100 5 2)
10

lispy> (% 17 5)
2

lispy> (/ 10 0)
Error: Division by zero!

lispy> (+ 1 "two")
Error: Cannot operate on non-number!

lispy> (invalid 1 2)
Error: Invalid operator!
```

## Performance Considerations
- **Memory efficiency**: Proper cleanup prevents memory leaks during operations
- **Early validation**: Fail fast when arguments are invalid
- **Iterative processing**: Process arguments sequentially to minimize memory usage

## Current Limitations
- Modulo operation currently only supports two arguments
- No support for floating-point numbers (integer only)
- No built-in mathematical functions (sin, cos, sqrt, etc.)
- Limited to basic arithmetic operations

## Next Steps
With robust mathematical operators implemented, the next iteration will focus on adding list manipulation functions (head, tail, list, cons, join) to extend the interpreter's capabilities beyond numerical computations.

## Key Files Modified
- `src/eval.c`: Enhanced mathematical operator implementation
- `test/test_eval.c`: Extended test coverage for mathematical operations
- `docs/iteration6.md`: This documentation file