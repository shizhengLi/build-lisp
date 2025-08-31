# 迭代8：环境和变量支持

## 概述
在本迭代中，我们实现了核心环境系统，支持变量定义和查找，这是Lisp编程的基础。这允许用户定义变量并在表达式中引用它们。

## 知识点

### 1. 环境结构
- **Lenv结构**：包含变量名（符号）数组及其对应值的数组
- **符号管理**：使用字符串高效存储和查找变量名
- **值存储**：正确存储不同类型Lisp值的内存管理

### 2. 变量操作
- **定义**：使用`def`特殊形式将值绑定到符号
- **查找**：从环境中检索绑定到符号的值
- **重新定义**：用新值更新现有变量绑定
- **错误处理**：优雅处理未定义变量引用

### 3. 特殊形式
- **def作为特殊形式**：与常规函数不同，`def`不以标准方式求值其参数
- **早期求值**：特殊形式在S表达式子节点的正常求值之前被处理
- **参数处理**：正确提取和验证特殊形式参数

## 实现细节

### 环境结构
```c
typedef struct Lenv {
    int count;
    char **syms;
    Lval **vals;
} Lenv;
```

### 核心环境函数

#### lenv_new
创建新的空环境：
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
在环境中查找符号：
```c
Lval *lenv_get(Lenv *e, Lval *k) {
    for (int i = 0; i < e->count; i++) {
        if (strcmp(e->syms[i], k->sym) == 0) {
            return lval_copy(e->vals[i]);
        }
    }
    return lval_err("未绑定符号！");
}
```

#### lenv_put
在环境中添加或更新变量：
```c
Lval *lenv_put(Lenv *e, Lval *k, Lval *v) {
    // 检查变量是否已存在
    for (int i = 0; i < e->count; i++) {
        if (strcmp(e->syms[i], k->sym) == 0) {
            lval_free(e->vals[i]);
            e->vals[i] = lval_copy(v);
            return lval_copy(v);
        }
    }
    
    // 如果未找到，添加新变量
    e->count++;
    e->syms = realloc(e->syms, sizeof(char*) * e->count);
    e->vals = realloc(e->vals, sizeof(Lval*) * e->count);
    
    e->syms[e->count - 1] = malloc(strlen(k->sym) + 1);
    strcpy(e->syms[e->count - 1], k->sym);
    e->vals[e->count - 1] = lval_copy(v);
    
    return lval_copy(v);
}
```

### 特殊形式处理
修改求值逻辑以将`def`作为特殊形式处理：
```c
Lval *eval_sexpr(Lenv *e, Lval *v) {
    // 在求值子节点之前检查特殊形式'def'
    if (v->sexpr.count > 0) {
        Lval *first = v->sexpr.cell[0];
        if (first->type == LVAL_SYM && strcmp(first->sym, "def") == 0) {
            return builtin_def(e, v);
        }
    }
    
    // 正常求值继续...
}
```

### 值复制
添加`lval_copy`函数以正确复制值：
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
        // ... 其他情况
    }
    
    return x;
}
```

## 挑战与解决方案

### 1. 特殊形式求值顺序
**问题**：`def`符号在特殊形式可以被处理之前就被求值，导致"未绑定符号！"错误。

**解决方案**：修改`eval_sexpr`在求值子节点之前检查特殊形式，允许`def`被特殊处理。

### 2. 参数处理
**问题**：`def`特殊形式接收整个S表达式包括`def`符号，但内置函数期望只接收变量名和值。

**解决方案**：修改`builtin_def`在处理实际参数之前正确提取和移除`def`符号。

### 3. 内存管理
**问题**：环境存储需要正确复制值以避免内存损坏和双重释放问题。

**解决方案**：实现`lval_copy`函数并确保存储在环境中的值具有正确的所有权语义。

### 4. 求值签名更改
**问题**：添加环境支持需要更改`eval`函数签名，破坏所有现有代码。

**解决方案**：系统地更新所有对`eval`的调用以包含环境参数，并更新REPL以创建和管理环境。

## 测试覆盖
创建了全面的测试，涵盖：
- 变量定义和值存储
- 变量查找和检索
- 变量重新定义和更新
- 未定义变量的错误处理
- 与表达式求值的集成

## 使用示例
环境系统支持变量使用：
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

## 当前限制
- 像`+`、`-`等运算符没有在环境中预定义
- 完整的S表达式解析尚未在REPL中实现
- 变量作用域仅限于单个全局环境

## 下一步
随着环境和变量支持的实现，下一个迭代将专注于添加条件语句和控制流结构如`if`表达式，实现更复杂的程序逻辑。