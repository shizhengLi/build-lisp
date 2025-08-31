# Iteration 7: List Built-in Functions

## Overview
In this iteration, we implemented the core list manipulation functions that are fundamental to Lisp programming: `head`, `tail`, `list`, `cons`, and `join`. These functions allow users to work with S-expressions as lists, enabling list processing capabilities.

## Knowledge Points

### 1. List Manipulation Functions
- **head**: Returns the first element of a list
- **tail**: Returns all elements except the first one
- **list**: Creates a list from its arguments
- **cons**: Prepends an element to a list
- **join**: Concatenates multiple lists into one

### 2. Memory Management
- Understanding ownership transfer between Lval objects
- Proper handling of nested memory structures
- Avoiding double-free errors in complex operations

### 3. Error Handling
- Validation of argument counts
- Type checking for list operations
- Handling edge cases (empty lists, invalid arguments)

## Implementation Details

### Built-in Function Architecture
We extended the evaluation system to recognize and handle built-in functions alongside mathematical operators:

```c
// In eval_sexpr function
if (strcmp(f->sym, "head") == 0) {
    result = builtin_head(v);
} else if (strcmp(f->sym, "tail") == 0) {
    result = builtin_tail(v);
} else if (strcmp(f->sym, "list") == 0) {
    result = builtin_list(v);
} else if (strcmp(f->sym, "cons") == 0) {
    result = builtin_cons(v);
} else if (strcmp(f->sym, "join") == 0) {
    result = builtin_join(v);
} else {
    result = builtin_op(v, f->sym);
}
```

### Key Functions Added

#### lval_add_front
Added a helper function to prepend elements to lists efficiently:

```c
Lval *lval_add_front(Lval *v, Lval *x) {
    v->sexpr.count++;
    v->sexpr.cell = realloc(v->sexpr.cell, sizeof(Lval*) * v->sexpr.count);
    memmove(&v->sexpr.cell[1], &v->sexpr.cell[0], sizeof(Lval*) * (v->sexpr.count - 1));
    v->sexpr.cell[0] = x;
    return v;
}
```

#### builtin_head
Extracts the first element from a list with proper error handling:

```c
Lval *builtin_head(Lval *a) {
    if (a->sexpr.count != 1) {
        lval_free(a);
        return lval_err("Function 'head' passed too many arguments!");
    }
    
    Lval *list = lval_pop(a, 0);
    if (list->type != LVAL_SEXPR) {
        lval_free(list);
        lval_free(a);
        return lval_err("Function 'head' passed incorrect type!");
    }
    
    if (list->sexpr.count == 0) {
        lval_free(list);
        lval_free(a);
        return lval_err("Function 'head' passed empty list!");
    }
    
    Lval *result = lval_pop(list, 0);
    lval_free(list);
    lval_free(a);
    return result;
}
```

## Challenges and Solutions

### 1. Memory Management Issues
**Problem**: Double-free errors occurred when test functions tried to free arguments that were already freed by built-in functions.

**Solution**: Understanding that built-in functions take ownership of their arguments and free them. Tests should only free the returned results.

### 2. Type Safety
**Problem**: Ensuring list operations only work on S-expressions and handle edge cases properly.

**Solution**: Comprehensive type checking and error messages for invalid inputs.

### 3. Nested Structure Access
**Problem**: Accessing nested fields in the Lval union structure correctly.

**Solution**: Using proper field access patterns like `result->sexpr.count` instead of `result->count`.

## Test Coverage
Added comprehensive tests covering:
- Basic functionality of each list operation
- Error conditions (empty lists, wrong types, invalid arguments)
- Integration with the evaluation system
- Memory management correctness

Total tests: 30 (6 new built-in function tests)

## Usage Examples
Once integrated into the REPL, users will be able to:
```lisp
lispy> (list 1 2 3)
(1 2 3)

lispy> (head (list 1 2 3))
1

lispy> (tail (list 1 2 3))
(2 3)

lispy> (cons 0 (list 1 2 3))
(0 1 2 3)

lispy> (join (list 1 2) (list 3 4))
(1 2 3 4)
```

## Next Steps
With list manipulation functions implemented, the next iteration will focus on adding environment and variable support, enabling users to define and use variables in their Lisp programs.