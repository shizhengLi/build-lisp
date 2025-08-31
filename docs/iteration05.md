# 迭代5：基本求值

## 概述
在本迭代中，我们实现了核心求值引擎，使我们的Lisp解释器真正运行起来。这使得通过将解析的抽象语法树转换为实际计算值来执行Lisp表达式成为可能。

## 知识点

### 1. 求值理论
- **符号求值**：符号在环境中查找以获取其绑定值
- **S表达式求值**：列表通过求值每个元素并将第一个元素作为函数应用于其余元素来求值
- **自求值形式**：数字、字符串和其他字面量求值为自身

### 2. 环境集成
- **环境参数**：所有求值函数现在都接受环境参数用于变量查找
- **符号解析**：求值符号时，咨询环境以查找绑定值
- **错误传播**：求值过程中的错误正确地在调用栈中向上传播

### 3. 函数应用
- **内置运算符**：数学运算符（+、-、*、/、%）实现为内置函数
- **参数验证**：函数验证其参数（类型检查、参数计数）
- **错误处理**：对无效操作的全面错误处理（除零、类型不匹配）

## 实现细节

### 核心求值函数
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

### S表达式求值
```c
Lval *eval_sexpr(Lenv *e, Lval *v) {
    // 求值子节点
    for (int i = 0; i < v->sexpr.count; i++) {
        v->sexpr.cell[i] = eval(e, v->sexpr.cell[i]);
    }
    
    // 错误检查
    for (int i = 0; i < v->sexpr.count; i++) {
        if (v->sexpr.cell[i]->type == LVAL_ERR) {
            return lval_take(v, i);
        }
    }
    
    // 空表达式
    if (v->sexpr.count == 0) {
        return v;
    }
    
    // 单个表达式
    if (v->sexpr.count == 1) {
        return lval_take(v, 0);
    }
    
    // 确保第一个元素是符号
    Lval *f = lval_pop(v, 0);
    if (f->type != LVAL_SYM) {
        lval_free(f);
        lval_free(v);
        return lval_err("S表达式不以符号开头！");
    }
    
    // 调用内置运算符
    Lval *result = builtin_op(e, v, f->sym);
    lval_free(f);
    return result;
}
```

### 内置运算符实现
```c
Lval *builtin_op(Lenv *e, Lval *a, char *op) {
    // 检查运算符是否有效
    if (strcmp(op, "+") != 0 && strcmp(op, "-") != 0 && 
        strcmp(op, "*") != 0 && strcmp(op, "/") != 0 && strcmp(op, "%") != 0) {
        lval_free(a);
        return lval_err("无效运算符！");
    }
    
    // 确保所有参数都是数字
    for (int i = 0; i < a->sexpr.count; i++) {
        if (a->sexpr.cell[i]->type != LVAL_NUM) {
            lval_free(a);
            return lval_err("不能对非数字进行运算！");
        }
    }
    
    Lval *x = lval_pop(a, 0);
    
    // 处理一元减号
    if ((strcmp(op, "-") == 0) && a->sexpr.count == 0) {
        x->num = -x->num;
    }
    
    // 当还有剩余元素时
    while (a->sexpr.count > 0) {
        Lval *y = lval_pop(a, 0);
        
        if (strcmp(op, "+") == 0) { x->num += y->num; }
        if (strcmp(op, "-") == 0) { x->num -= y->num; }
        if (strcmp(op, "*") == 0) { x->num *= y->num; }
        if (strcmp(op, "/") == 0) { 
            if (y->num == 0) {
                lval_free(x);
                lval_free(y);
                x = lval_err("除零错误！");
                break;
            }
            x->num /= y->num; 
        }
        if (strcmp(op, "%") == 0) { 
            if (y->num == 0) {
                lval_free(x);
                lval_free(y);
                x = lval_err("模零错误！");
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

## 挑战与解决方案

### 1. 环境集成
**问题**：求值系统需要与环境系统配合进行变量查找，但初始设计没有考虑到这一点。

**解决方案**：修改所有求值函数以接受环境参数，并使用`lenv_get`函数集成符号查找。

### 2. 错误传播
**问题**：求值过程中的错误需要正确传播而不会导致内存泄漏。

**解决方案**：在求值的每个阶段实施全面的错误检查，并使用`lval_take`函数正确处理返回错误时的内存管理。

### 3. 参数验证
**问题**：内置运算符需要在执行操作前验证其参数。

**解决方案**：添加类型检查以确保所有参数都是数字，并进行运算符验证以确保只执行支持的操作。

### 4. 内存管理
**问题**：具有多个临时值的复杂求值逻辑可能导致内存泄漏。

**解决方案**：谨慎使用`lval_pop`、`lval_take`和`lval_free`以确保所有中间值都被正确清理。

### 5. 一元运算符
**问题**：像`-`这样的数学运算符可以用作一元运算符（取负）或二元运算符（减法）。

**解决方案**：当运算符后只有一个参数时，添加特殊处理来处理一元减号。

## 测试覆盖
创建了全面的测试，涵盖：
- 数字和符号的基本求值
- 带数学运算的S表达式求值
- 所有数学运算符（+、-、*、/、%）
- 无效操作的错误处理
- 除零和模零错误
- 嵌套表达式求值
- 参数类型验证

## 当前状态
基本求值系统现已功能化，可以正确处理数学表达式和错误处理。解释器可以求值如`(+ 1 2 3)`、`(- 10 5)`、`(* 2 3 4)`、`(/ 100 5)`和`(% 17 5)`等表达式。

## 下一步
基本求值功能正常工作后，下一个迭代将专注于添加列表操作函数（head、tail、list、cons、join），以扩展解释器超越数学运算的能力。

## 关键修改文件
- `src/eval.c`：核心求值引擎实现
- `src/eval.h`：求值函数声明
- `test/test_eval.c`：求值功能的全面测试套件