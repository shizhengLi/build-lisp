# Building a Lisp Interpreter in C

  This is a Lisp interpreter project implemented in C, built through 11 iterations to create a complete Lisp interpreter
  with support for functions, lambda expressions, and a macro system.

  Project Overview

  This project builds a complete Lisp interpreter through 11 iterations, starting from basic mathematical expression
  evaluation and gradually adding variables, functions, lambda expressions, and finally implementing a complete macro
  system:

  1. Iteration 1: Basic Syntax Parser - Implement simple mathematical expression evaluation
  2. Iteration 2: Environment Variables - Add variable storage and retrieval
  3. Iteration 3: S-Expressions - Support nested S-expressions
  4. Iteration 4: Symbol Computation - Add symbol processing
  5. Iteration 5: Function System - Implement basic function calls
  6. Iteration 6: Conditional Expressions - Add if conditional statements
  7. Iteration 7: Recursive Functions - Support recursive function definition and calls
  8. Iteration 8: Error Handling - Improve error handling mechanisms
  9. Iteration 9: Garbage Collection - Implement automatic memory management
  10. Iteration 10: Lambda Expressions - Implement anonymous functions
  11. Iteration 11: Macro System - Implement macro expressions and code generation

  Quick Start

  Compile the Project

  make

  Run the Interpreter

  ./lispy

  Run Tests

  make test

  Memory Check

  make memcheck

  Project Structure

  .
  ├── Makefile              # Build configuration
  ├── README.md            # Project documentation (Chinese)
  ├── README.en.md         # Project documentation (English)
  ├── lispy               # Executable Lisp interpreter
  ├── docs/               # Documentation directory
  │   ├── 技术文档.md     # Technical implementation (Chinese)
  │   ├── 迭代文档.md     # Iteration development records (Chinese)
  │   ├── 宏系统文档.md   # Macro system documentation (Chinese)
  │   └── 近代理档.md     # Recent development records (Chinese)
  ├── src/                # Source code directory
  │   ├── env.c          # Environment variable management
  │   ├── env.h          # Environment variable header
  │   ├── eval.c         # Expression evaluation
  │   ├── eval.h         # Evaluation header
  │   ├── lval.c         # Lisp value type implementation
  │   ├── lval.h         # Lisp value type header
  │   ├── main.c         # Main program entry
  │   ├── parser.c       # Syntax parser
  │   └── parser.h       # Syntax parser header
  └── test/               # Test directory
      ├── test_macro.c   # Macro system tests
      ├── test_runner.c  # Test runner
      └── test_macro_only.c # Macro system standalone tests

  Features

  Supported Data Types

  - Integers: Numeric type supporting arithmetic operations
  - Symbols: Variable names and function names
  - S-Expressions: Lists and nested structures
  - Strings: Text data
  - Errors: Error messages
  - Functions: Built-in and user-defined functions
  - Lambda Expressions: Anonymous functions with closure support
  - Macro Expressions: Code generation and metaprogramming

  Supported Operations

  Arithmetic Operations

  lispy> (+ 1 2 3 4)
  10
  lispy> (- 10 3)
  7
  lispy> (* 2 3 4)
  24
  lispy> (/ 10 2)
  5
  lispy> (% 10 3)
  1

  Comparison Operations

  lispy> (> 5 3)
  1
  lispy> (< 5 3)
  0
  lispy> (= 5 5)
  1
  lispy> (>= 5 5)
  1
  lispy> (<= 3 5)
  1

  List Operations

  lispy> (head (list 1 2 3 4))
  1
  lispy> (tail (list 1 2 3 4))
  (2 3 4)
  lispy> (list 1 2 3)
  (1 2 3)
  lispy> (cons 1 (list 2 3))
  (1 2 3)
  lispy> (join (list 1 2) (list 3 4))
  (1 2 3 4)

  Conditional Expressions

  lispy> (if (> 2 1) "yes" "no")
  "yes"
  lispy> (if (< 2 1) "yes" "no")
  "no"

  Variable Binding

  lispy> (def x 10)
  10
  lispy> (+ x 5)
  15
  lispy> (def y 20)
  20
  lispy> (+ x y)
  30

  Function Definition

  lispy> (def add (lambda (a b) (+ a b)))
  <function add>
  lispy> (add 3 4)
  7
  lispy> (def square (lambda (x) (* x x)))
  <function square>
  lispy> (square 5)
  25

  Lambda Expressions

  lispy> ((lambda (x) (* x x)) 5)
  25
  lispy> ((lambda (a b) (+ a b)) 3 4)
  7

  Macro System (Iteration 11 Feature)

  lispy> (def unless (macro (cond body) (list 'if (list 'not cond) body)))
  <macro unless>
  lispy> (unless (= 2 2) (println "not equal"))

  lispy> (unless (= 2 3) (println "not equal"))
  "not equal"

  lispy> (def when (macro (cond body) (list 'if cond body)))
  <macro when>
  lispy> (when (= 2 2) (println "equal"))
  "equal"
  lispy> (when (= 2 3) (println "equal"))

  Technical Implementation

  Core Data Structures

  Lval (Lisp Value)

  Lval is the core data structure of the Lisp interpreter, using unions to implement unified representation of multiple
  data types:
```c
  typedef struct Lval {
      LvalType type;
      union {
          long num;           // Numbers
          char *sym;          // Symbols
          char *err;          // Errors
          char *str;          // Strings
          struct {
              struct Lval *cell;   // S-expression cells
              int count;         // Cell count
          } sexpr;
          struct {
              struct Lval *formals; // Formal parameters
              struct Lval *body;    // Function body
              Lenv *env;         // Environment
          } lambda;
          struct {
              struct Lval *formals; // Formal parameters
              struct Lval *body;    // Macro body
              Lenv *env;         // Environment
          } macro;
      };
  } Lval;

  Lenv (Lisp Environment)

  Lenv is the Lisp environment structure, using linked structure to implement scopes:

  typedef struct Lenv {
      int count;
      char **syms;
      struct Lval **vals;
      struct Lenv *par;
  } Lenv;
  ```

  Main Modules

  1. Syntax Parser (parser.c)

  Responsible for converting input strings into Lisp abstract syntax trees (AST).
  - Number parsing: parse_number
  - Symbol parsing: parse_symbol
  - S-expression parsing: parse_sexpr
  - String parsing: parse_string

  2. Evaluator (eval.c)

  Responsible for evaluating Lisp expressions.
  - Symbol evaluation: eval_symbol
  - S-expression evaluation: eval_sexpr
  - Function calling: lval_call
  - Conditional expressions: builtin_if

  3. Environment Management (env.c)

  Responsible for managing Lisp variable environments.
  - Environment creation: lenv_new
  - Variable binding: lenv_def
  - Variable lookup: lenv_get
  - Variable update: lenv_put

  4. Value Type Management (lval.c)

  Responsible for managing various Lisp value types.
  - Value creation: lval_num, lval_sym, lval_sexpr
  - Value copying: lval_copy
  - Value freeing: lval_free
  - Value conversion: lval_to_string

  Macro System Implementation

  Difference Between Macros and Functions

  - Functions: Arguments are evaluated first, then function is applied
  - Macros: Arguments are not evaluated, directly expanded, then result is evaluated

  Macro Creation and Evaluation
```c
  // Macro creation
  Lval *builtin_macro(Lenv *e, Lval *a) {
      // Parameter validation and macro creation
      return lval_macro(formals, body, e);
  }

  // Macro evaluation - key is that arguments are not evaluated, directly expanded
  if (f->type == LVAL_MACRO) {
      // Bind arguments to macro environment (without evaluation)
      Lenv *macro_env = lenv_new();
      macro_env->par = f->macro.env;
      for (int i = 0; i < f->macro.formals->sexpr.count; i++) {
          lenv_put(macro_env, f->macro.formals->sexpr.cell[i], args->sexpr.cell[i]);
      }

      // Evaluate macro body in macro environment
      Lval *expanded = eval(macro_env, f->macro.body);
      lenv_del(macro_env);

      // Evaluate the expanded result again
      Lval *result = eval(e, expanded);
      lval_free(expanded);
      return result;
  }
```
  Development and Testing

  Build Options

  make clean      # Clean build files
  make docs      # Generate documentation
  make memcheck  # Memory leak check

  Test Coverage

  The project includes a complete test suite:
  - Unit Tests: Tests for individual modules
  - Integration Tests: Tests for overall functionality
  - Macro System Tests: Specialized tests for macro functionality

  Run tests:
  make test

  Sample test results:
  ./test_runner

  Running basic tests...
  ✓ PASSED: test_number_representation
  ✓ PASSED: test_symbol_representation
  ✓ PASSED: test_sexpr_representation
  ✓ PASSED: test_string_representation
  ✓ PASSED: test_eval_basic_arithmetic
  ✓ PASSED: test_eval_variable_lookup
  ✓ PASSED: test_eval_function_call
  ✓ PASSED: test_eval_conditional
  ✓ PASSED: test_eval_lambda
  ✓ PASSED: test_eval_macro
  ✓ PASSED: test_macro_with_multiple_params
  ✓ PASSED: test_macro_quote_demonstration
  ✓ PASSED: test_macro_code_generation
  ✓ PASSED: test_macro_error_handling

  All tests passed! 🎉

  Memory Management

  Manual Memory Management

  The project uses manual memory management, with memory allocation and deallocation through malloc and free.

  Memory Leak Detection

  Use Valgrind for memory leak detection:
  make memcheck

  Key Memory Management Functions

  - lval_free: Free Lval and its substructures
  - lval_copy: Deep copy Lval
  - lenv_del: Free environment and its contents

  Documentation

  Detailed technical documentation can be found in the docs/ directory:

  - Iteration Documentation: Detailed explanation and technical implementation of each iteration
  - Technical Documentation: Technical implementation details of core modules
  - Macro System Documentation: Detailed explanation and usage examples of the macro system
  - Recent Development Documentation: Records of recent development activities

  Example Programs

  Recursive Functions

  lispy> (def factorial (lambda (n)
      (if (<= n 1)
          1
          (* n (factorial (- n 1))))))
  <function factorial>
  lispy> (factorial 5)
  120

  Higher-Order Functions

  lispy> (def map (lambda (f lst)
      (if (= (tail lst) ())
          ()
          (cons (f (head lst)) (map f (tail lst))))))
  <function map>
  lispy> (map (lambda (x) (* x x)) (list 1 2 3 4))
  (1 4 9 16)

  Macro Metaprogramming Capabilities

  lispy> (def for-loop (macro (var start end body)
      (list 'lambda (list var)
          (list 'if (list '> var end)
              '()
              (list 'cons body (list 'for-loop var (list '+ var 1) body)))))))
  <macro for-loop>
  lispy> ((for-loop 'i 1 3) (print i))
  1
  2
  3

  Contributing

  Issues and Pull Requests are welcome to improve this project.

  License

  This project is licensed under the MIT License.
