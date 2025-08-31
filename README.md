# 用C语言构建Lisp解释器

  这是一个用C语言实现的Lisp解释器项目，通过11次迭代逐步构建一个完整的Lisp解释器，支持函数、Lambda表达式和宏系统。

  项目概述

  本项目通过11次迭代逐步构建一个完整的Lisp解释器，从基础的数学表达式求值开始，逐步添加变量、函数、Lambda表达式，最终实现完
  整的宏系统：

  1. 迭代 1: 基础语法解析器 - 实现简单的数学表达式求值
  2. 迭代 2: 环境变量 - 添加变量存储和检索功能
  3. 迭代 3: S-表达式 - 支持嵌套的S-表达式
  4. 迭代 4: 符号计算 - 添加符号处理功能
  5. 迭代 5: 函数系统 - 实现基础函数调用
  6. 迭代 6: 条件表达式 - 添加if条件判断
  7. 迭代 7: 递归函数 - 支持递归函数定义和调用
  8. 迭代 8: 错误处理 - 完善错误处理机制
  9. 迭代 9: 垃圾回收 - 实现自动内存管理
  10. 迭代 10: Lambda表达式 - 实现匿名函数
  11. 迭代 11: 宏系统 - 实现宏表达式和代码生成

  快速开始

  编译项目

  make

  运行解释器

  ./lispy

  运行测试

  make test

  内存检查

  make memcheck

  项目结构

  ```.
  ├── Makefile              # 编译配置
  ├── README.md            # 项目说明（中文）
  ├── README.en.md         # 项目说明（英文）
  ├── lispy               # 可执行的Lisp解释器
  ├── docs/               # 文档目录
  │   ├── 技术文档.md     # 核心技术实现
  │   ├── 迭代文档.md     # 迭代开发记录
  │   ├── 宏系统文档.md   # 宏系统详细说明
  │   └── 近代理档.md     # 最近开发记录
  ├── src/                # 源代码目录
  │   ├── env.c          # 环境变量管理
  │   ├── env.h          # 环境变量头文件
  │   ├── eval.c         # 表达式求值
  │   ├── eval.h         # 求值头文件
  │   ├── lval.c         # Lisp值类型实现
  │   ├── lval.h         # Lisp值类型头文件
  │   ├── main.c         # 主程序入口
  │   ├── parser.c       # 语法解析器
  │   └── parser.h       # 语法解析头文件
  └── test/               # 测试目录
      ├── test_macro.c   # 宏系统测试
      ├── test_runner.c  # 测试运行器
      └── test_macro_only.c # 宏系统独立测试
```
  功能特性

  支持的数据类型

  - 整数: 数值类型，支持算术运算
  - 符号: 变量名和函数名
  - S-表达式: 列表和嵌套结构
  - 字符串: 文本数据
  - 错误: 错误信息
  - 函数: 内置函数和用户定义函数
  - Lambda表达式: 匿名函数，支持闭包
  - 宏表达式: 代码生成和元编程

  支持的操作

  算术运算

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

  比较运算

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

  列表操作

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

  条件表达式

  lispy> (if (> 2 1) "yes" "no")
  "yes"
  lispy> (if (< 2 1) "yes" "no")
  "no"

  变量绑定

  lispy> (def x 10)
  10
  lispy> (+ x 5)
  15
  lispy> (def y 20)
  20
  lispy> (+ x y)
  30

  函数定义

  lispy> (def add (lambda (a b) (+ a b)))
  <function add>
  lispy> (add 3 4)
  7
  lispy> (def square (lambda (x) (* x x)))
  <function square>
  lispy> (square 5)
  25

  Lambda表达式

  lispy> ((lambda (x) (* x x)) 5)
  25
  lispy> ((lambda (a b) (+ a b)) 3 4)
  7

  宏系统（迭代11特色功能）

  lispy> (def unless (macro (cond body) (list 'if (list 'not cond) body)))
  <macro unless>
  lispy> (unless (= 2 2) (println "不相等"))

  lispy> (unless (= 2 3) (println "不相等"))
  "不相等"

  lispy> (def when (macro (cond body) (list 'if cond body)))
  <macro when>
  lispy> (when (= 2 2) (println "相等"))
  "相等"
  lispy> (when (= 2 3) (println "相等"))

  技术实现

  核心数据结构

  Lval (Lisp Value)

  Lval 是Lisp解释器的核心数据结构，使用联合体（union）实现多种数据类型的统一表示：
```c
  typedef struct Lval {
      LvalType type;
      union {
          long num;           // 数字
          char *sym;          // 符号
          char *err;          // 错误
          char *str;          // 字符串
          struct {
              struct Lval *cell;   // S-表达式单元
              int count;         // 单元数量
          } sexpr;
          struct {
              struct Lval *formals; // 形式参数
              struct Lval *body;    // 函数体
              Lenv *env;         // 环境
          } lambda;
          struct {
              struct Lval *formals; // 形式参数
              struct Lval *body;    // 宏体
              Lenv *env;         // 环境
          } macro;
      };
  } Lval;

  Lenv (Lisp Environment)

  Lenv 是Lisp的环境结构，使用链式结构实现作用域：

  typedef struct Lenv {
      int count;
      char **syms;
      struct Lval **vals;
      struct Lenv *par;
  } Lenv;
``` 
  主要模块

  1. 语法解析器 (parser.c)

  负责将输入的字符串转换为Lisp的抽象语法树(AST)。
  - 数字解析：parse_number
  - 符号解析：parse_symbol
  - S-表达式解析：parse_sexpr
  - 字符串解析：parse_string

  2. 求值器 (eval.c)

  负责对Lisp表达式进行求值。
  - 符号求值：eval_symbol
  - S-表达式求值：eval_sexpr
  - 函数调用：lval_call
  - 条件表达式：builtin_if

  3. 环境管理 (env.c)

  负责管理Lisp的变量环境。
  - 环境创建：lenv_new
  - 变量绑定：lenv_def
  - 变量查找：lenv_get
  - 变量更新：lenv_put

  4. 值类型管理 (lval.c)

  负责管理Lisp的各种值类型。
  - 值创建：lval_num, lval_sym, lval_sexpr
  - 值复制：lval_copy
  - 值释放：lval_free
  - 值转换：lval_to_string

  宏系统实现

  宏与函数的区别

  - 函数：参数先求值，再应用函数
  - 宏：参数不求值，直接展开，然后对结果求值

  宏的创建和求值
```c
  // 宏创建
  Lval *builtin_macro(Lenv *e, Lval *a) {
      // 参数验证和宏创建
      return lval_macro(formals, body, e);
  }

  // 宏求值 - 关键在于参数不求值，直接展开
  if (f->type == LVAL_MACRO) {
      // 将参数绑定到宏环境（不求值）
      Lenv *macro_env = lenv_new();
      macro_env->par = f->macro.env;
      for (int i = 0; i < f->macro.formals->sexpr.count; i++) {
          lenv_put(macro_env, f->macro.formals->sexpr.cell[i], args->sexpr.cell[i]);
      }

      // 在宏环境中求值宏体
      Lval *expanded = eval(macro_env, f->macro.body);
      lenv_del(macro_env);

      // 对展开的结果再次求值
      Lval *result = eval(e, expanded);
      lval_free(expanded);
      return result;
  }
  ```

  开发和测试

  编译选项

  make clean      # 清理编译文件
  make docs      # 生成文档
  make memcheck  # 内存泄漏检查

  测试覆盖

  项目包含完整的测试套件：
  - 单元测试: 针对各个模块的测试
  - 集成测试: 针对整体功能的测试
  - 宏系统测试: 专门针对宏功能的测试

  运行测试：
  make test

  测试结果示例：
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

  内存管理

  手动内存管理

  项目使用手动内存管理，通过 malloc 和 free 进行内存分配和释放。

  内存泄漏检测

  使用 Valgrind 进行内存泄漏检测：
  make memcheck

  关键内存管理函数

  - lval_free: 释放Lval及其子结构
  - lval_copy: 深度复制Lval
  - lenv_del: 释放环境及其内容

  文档

  详细的技术文档请查看 docs/ 目录：

  - 迭代文档: 每次迭代的详细说明和技术实现
  - 技术文档: 核心模块的技术实现细节
  - 宏系统文档: 宏系统的详细说明和使用示例
  - 近代理档: 最近开发的内容记录

  示例程序

  递归函数

  lispy> (def factorial (lambda (n)
      (if (<= n 1)
          1
          (* n (factorial (- n 1))))))
  <function factorial>
  lispy> (factorial 5)
  120

  高阶函数

  lispy> (def map (lambda (f lst)
      (if (= (tail lst) ())
          ()
          (cons (f (head lst)) (map f (tail lst))))))
  <function map>
  lispy> (map (lambda (x) (* x x)) (list 1 2 3 4))
  (1 4 9 16)

  宏的元编程能力

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

  贡献

  欢迎提交Issue和Pull Request来改进这个项目。

  许可证

  本项目采用MIT许可证。

