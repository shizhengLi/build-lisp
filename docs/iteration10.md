# 迭代 10: 函数与 Lambda

## 概述

迭代 10 实现了 Lisp 解释器中的函数和 Lambda 表达式功能。这是整个项目的重要里程碑，为后续的宏系统实现奠定了基础。

## 实现的功能

### 1. Lambda 表达式语法

支持使用 `\` 符号创建匿名函数：

```lisp
(\ {参数} {函数体})
```

示例：
```lisp
(\ {x} {+ x 1})    ; 接受一个参数 x，返回 x + 1
(\ {x y} {+ x y})  ; 接受两个参数，返回它们的和
```

### 2. 函数调用

Lambda 表达式可以作为函数直接调用：

```lisp
((\ {x} {+ x 1}) 5)  ; 返回 6
((\ {x y} {+ x y}) 3 4)  ; 返回 7
```

### 3. 闭包 (Closure)

Lambda 表达式支持闭包，可以捕获创建时的环境变量：

```lisp
(def {x} 10)
((\ {y} {+ x y}) 5)  ; 返回 15 (捕获了 x = 10)
```

### 4. 递归支持

Lambda 表达式支持递归调用：

```lisp
((\ {n} {if (= n 0) {1} {* n ((\ {n} {if (= n 0) {1} {* n ((\ {n} {if (= n 0) {1} {* n ...}})}})}) (- n 1))}}) 5)
```

## 技术实现

### 1. 数据结构扩展

在 `lval.h` 中添加了新的类型和结构：

```c
typedef enum {
    LVAL_NUM,
    LVAL_SYM,
    LVAL_SEXPR,
    LVAL_ERR,
    LVAL_FUN,
    LVAL_LAMBDA    // 新增：Lambda 类型
} LvalType;

typedef struct Lval {
    LvalType type;
    union {
        // ... 其他类型
        struct {
            struct Lval *formals;  // 形式参数列表
            struct Lval *body;     // 函数体
            Lenv *env;             // 闭包环境
        } lambda;
    };
} Lval;
```

### 2. 环境层次结构

扩展了环境结构以支持父环境引用：

```c
typedef struct Lenv {
    int count;
    char **syms;
    Lval **vals;
    struct Lenv *parent;  // 父环境，用于闭包
} Lenv;
```

### 3. 核心函数

#### `lval_lambda()` - 创建 Lambda 函数

```c
Lval *lval_lambda(Lval *formals, Lval *body, Lenv *env);
```

#### `builtin_lambda()` - Lambda 特殊形式处理

```c
Lval *builtin_lambda(Lenv *e, Lval *a);
```

#### `lval_call()` - 统一函数调用机制

```c
Lval *lval_call(Lenv *e, Lval *f, Lval *a);
```

### 4. 求值流程

1. **Lambda 创建**：
   - 解析 `\` 特殊形式
   - 验证参数为符号列表
   - 创建 Lambda 结构体
   - 捕获当前环境

2. **函数调用**：
   - 检查函数类型（内置函数或 Lambda）
   - 对于 Lambda：
     - 创建新环境，父环境为闭包环境
     - 绑定实际参数到形式参数
     - 在新环境中求值函数体

### 5. 内存管理

- Lambda 表达式持有环境的引用，但不拥有它
- 函数调用时创建的新环境在调用结束后被释放
- 参数绑定时使用值拷贝避免内存问题

## 测试用例

### 1. 基本 Lambda 创建和调用

```c
// 测试：(\ {x} {+ x 1}) 5 应该返回 6
Lval *expr = lval_sexpr();
lval_add(expr, lval_sexpr());  // lambda
lval_add(expr->sexpr.cell[0], lval_sym("\\"));
lval_add(expr->sexpr.cell[0], lval_sexpr());  // params
lval_add(expr->sexpr.cell[0]->sexpr.cell[1], lval_sym("x"));
lval_add(expr->sexpr.cell[0], lval_sexpr());  // body
lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("+"));
lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_sym("x"));
lval_add(expr->sexpr.cell[0]->sexpr.cell[2], lval_num(1));
lval_add(expr, lval_num(5));  // argument

Lval *result = eval(e, expr);
// result->num 应该等于 6
```

### 2. 多参数 Lambda

```c
// 测试：(\ {x y} {+ x y}) 3 4 应该返回 7
// 类似结构，但有两个参数
```

### 3. 闭包测试

```c
// 测试：捕获外部变量
Lval *def_expr = lval_sexpr();
lval_add(def_expr, lval_sym("def"));
lval_add(def_expr, lval_sym("x"));
lval_add(def_expr, lval_num(10));
eval(e, def_expr);

// Lambda 应该能访问到 x = 10
```

### 4. 错误处理

```c
// 测试：参数数量不匹配
// 测试：参数类型错误
// 测试：未定义变量访问
```

## 遇到的挑战和解决方案

### 1. 内存管理问题

**问题**：Lambda 函数调用时出现双重释放错误。

**解决方案**：
- 明确内存所有权
- Lambda 表达式不拥有环境，只是引用
- 函数调用时创建的新环境独立管理

### 2. 环境层次结构

**问题**：需要实现闭包，Lambda 需要访问定义时的环境。

**解决方案**：
- 在环境结构中添加父环境指针
- 实现环境链式查找
- 创建 Lambda 时捕获当前环境

### 3. 函数调用统一接口

**问题**：需要统一处理内置函数和 Lambda 函数的调用。

**解决方案**：
- 创建 `lval_call()` 统一接口
- 根据函数类型分发到不同处理逻辑
- 保持内置函数的性能优势

## 性能考虑

1. **环境查找**：链式查找可能影响性能，但符合 Lisp 语义
2. **内存分配**：每次函数调用都创建新环境，但生命周期短
3. **参数传递**：使用值拷贝确保安全性

## 后续改进方向

1. **尾调用优化**：避免递归调用的栈溢出
2. **更高效的内存管理**：考虑垃圾回收
3. **函数组合**：支持函数式编程特性
4. **类型系统**：更强的类型检查

## 总结

迭代 10 成功实现了 Lambda 表达式和闭包功能，这是 Lisp 解释器的核心特性之一。通过合理的数据结构设计和内存管理，我们实现了：

- 完整的 Lambda 表达式语法
- 正确的闭包语义
- 统一的函数调用机制
- 良好的错误处理

这为下一阶段的宏系统实现打下了坚实的基础。