# Iteration 8: Environment and Variables Support

## Overview
In this iteration, we implemented a core environment system that enables variable definition and lookup, which is fundamental to Lisp programming. This allows users to define variables and reference them in expressions.

## Knowledge Points

### 1. Environment Structure
- **Lenv structure**: Contains arrays for variable names (symbols) and their corresponding values
- **Symbol management**: Efficient storage and lookup of variable names using strings
- **Value storage**: Proper memory management for storing different types of Lisp values

### 2. Variable Operations
- **Definition**: Using the `def` special form to bind values to symbols
- **Lookup**: Retrieving values bound to symbols from the environment
- **Redefinition**: Updating existing variable bindings with new values
- **Error handling**: Graceful handling of undefined variable references

### 3. Special Forms
- **def as a special form**: Unlike regular functions, `def` doesn't evaluate its arguments in the standard way
- **Early evaluation**: Special forms are processed before normal evaluation of S-expression children
- **Argument handling**: Proper extraction and validation of special form arguments

## Implementation Details

### Environment Structure
```c
typedef struct Lenv {
    int count;
    char **syms;
    Lval **vals;
} Lenv;
```

### Core Environment Functions

#### lenv_new
Creates a new empty environment:
```c
Lenv *lenv_new(void) {
    Lenv *e = malloc(sizeof(Lenv));
    e->count = 0;
    e->syms = NULL;
    e->vals = NULL;
    return e;
}
```

#### lenv_get
Looks up a symbol in the environment:
```c
Lval *lenv_get(Lenv *e, Lval *k) {
    for (int i = 0; i < e->count; i++) {
        if (strcmp(e->syms[i], k->sym) == 0) {
            return lval_copy(e->vals[i]);
        }
    }
    return lval_err("Unbound symbol!");
}
```

#### lenv_put
Adds or updates a variable in the environment:
```c
Lval *lenv_put(Lenv *e, Lval *k, Lval *v) {
    // Check if variable already exists
    for (int i = 0; i < e->count; i++) {
        if (strcmp(e->syms[i], k->sym) == 0) {
            lval_free(e->vals[i]);
            e->vals[i] = lval_copy(v);
            return lval_copy(v);
        }
    }
    
    // If not found, add new variable
    e->count++;
    e->syms = realloc(e->syms, sizeof(char*) * e->count);
    e->vals = realloc(e->vals, sizeof(Lval*) * e->count);
    
    e->syms[e->count - 1] = malloc(strlen(k->sym) + 1);
    strcpy(e->syms[e->count - 1], k->sym);
    e->vals[e->count - 1] = lval_copy(v);
    
    return lval_copy(v);
}
```

### Special Form Handling
Modified evaluation logic to handle `def` as a special form:
```c
Lval *eval_sexpr(Lenv *e, Lval *v) {
    // Check for special form 'def' before evaluating children
    if (v->sexpr.count > 0) {
        Lval *first = v->sexpr.cell[0];
        if (first->type == LVAL_SYM && strcmp(first->sym, "def") == 0) {
            return builtin_def(e, v);
        }
    }
    
    // Normal evaluation continues...
}
```

### Value Copying
Added `lval_copy` function for proper value duplication:
```c
Lval *lval_copy(Lval *v) {
    Lval *x = malloc(sizeof(Lval));
    x->type = v->type;
    
    switch (v->type) {
        case LVAL_NUM:
            x->num = v->num;
            break;
        case LVAL_SYM:
            x->sym = malloc(strlen(v->sym) + 1);
            strcpy(x->sym, v->sym);
            break;
        // ... other cases
    }
    
    return x;
}
```

## Challenges and Solutions

### 1. Special Form Evaluation Order
**Problem**: The `def` symbol was being evaluated before the special form could be processed, causing "Unbound symbol!" errors.

**Solution**: Modified `eval_sexpr` to check for special forms before evaluating children, allowing `def` to be handled specially.

### 2. Argument Processing
**Problem**: The `def` special form receives the entire S-expression including the `def` symbol, but the builtin function expected only the variable name and value.

**Solution**: Modified `builtin_def` to properly extract and remove the `def` symbol before processing the actual arguments.

### 3. Memory Management
**Problem**: Environment storage needed proper copying of values to avoid memory corruption and double-free issues.

**Solution**: Implemented `lval_copy` function and ensured proper ownership semantics for values stored in the environment.

### 4. Evaluation Signature Changes
**Problem**: Adding environment support required changing the `eval` function signature, breaking all existing code.

**Solution**: Systematically updated all calls to `eval` to include the environment parameter and updated the REPL to create and manage an environment.

## Test Coverage
Created comprehensive tests covering:
- Variable definition and value storage
- Variable lookup and retrieval
- Variable redefinition and updates
- Error handling for undefined variables
- Integration with expression evaluation

## Usage Examples
The environment system enables variable usage:
```lisp
lispy> (def x 42)
42

lispy> x
42

lispy> (def y 100)
100

lispy> y
100
```

## Current Limitations
- Operators like `+`, `-`, etc. are not pre-defined in the environment
- Full S-expression parsing is not yet implemented in the REPL
- Variable scope is limited to a single global environment

## Next Steps
With environment and variable support implemented, the next iteration will focus on adding conditionals and control flow structures like `if` expressions, enabling more complex program logic.