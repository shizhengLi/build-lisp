# 迭代 7：列表内置函数

## 概述
在本迭代中，我们实现了Lisp编程中基本的列表操作函数：`head`、`tail`、`list`、`cons` 和 `join`。这些函数允许用户将S表达式作为列表来处理，提供了列表处理的能力。

## 知识点

### 1. 列表操作函数
- **head**：返回列表的第一个元素
- **tail**：返回除第一个元素外的所有元素
- **list**：从参数创建列表
- **cons**：将元素添加到列表头部
- **join**：将多个列表连接成一个列表

### 2. 内存管理
- 理解Lval对象之间的所有权转移
- 正确处理嵌套内存结构
- 在复杂操作中避免重复释放错误

### 3. 错误处理
- 参数数量验证
- 列表操作的类型检查
- 处理边界情况（空列表、无效参数）

## 实现细节

### 内置函数架构
我们扩展了求值系统，使其能够识别和处理内置函数以及数学运算符：

```c
// 在 eval_sexpr 函数中
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

### 添加的关键函数

#### lval_add_front
添加了一个辅助函数来高效地在列表头部添加元素：

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
从列表中提取第一个元素，并进行适当的错误处理：

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

## 挑战与解决方案

### 1. 内存管理问题
**问题**：当测试函数尝试释放已被内置函数释放的参数时，发生了重复释放错误。

**解决方案**：理解内置函数会拥有其参数的所有权并释放它们。测试应该只释放返回的结果。

### 2. 类型安全
**问题**：确保列表操作只在S表达式上工作，并正确处理边界情况。

**解决方案**：对无效输入进行全面的类型检查和错误消息。

### 3. 嵌套结构访问
**问题**：正确访问Lval联合结构中的嵌套字段。

**解决方案**：使用正确的字段访问模式，如 `result->sexpr.count` 而不是 `result->count`。

## 测试覆盖
添加了全面的测试，涵盖：
- 每个列表操作的基本功能
- 错误条件（空列表、错误类型、无效参数）
- 与求值系统的集成
- 内存管理正确性

总测试数：30（6个新的内置函数测试）

## 使用示例
一旦集成到REPL中，用户将能够：
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

## 下一步
随着列表操作函数的实现，下一个迭代将专注于添加环境和变量支持，使用户能够在他们的Lisp程序中定义和使用变量。