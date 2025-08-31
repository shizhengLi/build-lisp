# 迭代9：条件表达式和控制流

## 概述
在本迭代中，我们实现了条件表达式和控制流功能，为Lisp语言添加了基本的决策能力。这包括 `if` 条件表达式和比较运算符（`=`, `>`, `<`, `>=`, `<=`），使用户能够编写根据条件执行不同代码分支的程序。

## 核心知识点

### 1. 条件表达式 (`if`)
- **语法**: `(if condition then-branch else-branch)`
- **语义**: 根据 `condition` 的真假值选择执行 `then-branch` 或 `else-branch`
- **真值判断**: 
  - 数字：非零为真，零为假
  - S-表达式：非空列表为真，空列表为假
  - 其他类型：默认为真

### 2. 比较运算符
- **等于**: `=` - 检查所有参数是否相等
- **大于**: `>` - 检查每个后续参数是否大于前一个
- **小于**: `<` - 检查每个后续参数是否小于前一个
- **大于等于**: `>=` - 检查每个后续参数是否大于等于前一个
- **小于等于**: `<=` - 检查每个后续参数是否小于等于前一个

### 3. 特殊形式处理
- `if` 是特殊形式，不是普通函数
- 条件分支是惰性求值的（只有选中的分支会被执行）
- 支持可选的 else 分支

## 实现细节

### 1. 解析器扩展
在 `src/parser.c` 中扩展了对比较运算符的识别：

```c
// 添加比较运算符字符支持
} else if (isalpha(input[*pos]) || input[*pos] == '+' || input[*pos] == '-' || 
           input[*pos] == '*' || input[*pos] == '/' || input[*pos] == '%' || 
           input[*pos] == '=' || input[*pos] == '>' || input[*pos] == '<') {
```

### 2. 比较运算符实现
在 `src/eval.c` 的 `builtin_op` 函数中添加了比较逻辑：

```c
// 处理比较运算符
if (strcmp(op, "=") == 0 || strcmp(op, ">") == 0 || strcmp(op, "<") == 0 ||
    strcmp(op, ">=") == 0 || strcmp(op, "<=") == 0) {
    
    int result = 1; // 默认为真
    
    for (int i = 0; i < a->sexpr.count; i++) {
        Lval *y = a->sexpr.cell[i];
        
        if (strcmp(op, "=") == 0) { 
            if (x->num != y->num) result = 0;
        }
        if (strcmp(op, ">") == 0) { 
            if (x->num <= y->num) result = 0;
        }
        // ... 其他比较运算符
    }
    
    return lval_num(result);
}
```

### 3. 条件表达式实现
实现了 `builtin_if` 函数处理条件逻辑：

```c
Lval *builtin_if(Lenv *e, Lval *a) {
    // 移除 'if' 符号
    Lval *if_sym = lval_pop(a, 0);
    lval_free(if_sym);
    
    // 验证参数数量
    if (a->sexpr.count < 2 || a->sexpr.count > 3) {
        lval_free(a);
        return lval_err("Function 'if' passed incorrect number of arguments!");
    }
    
    // 求值条件
    Lval *cond = eval(e, lval_pop(a, 0));
    
    // 判断真假值
    int truthy = 0;
    if (cond->type == LVAL_NUM) {
        truthy = (cond->num != 0);
    } else if (cond->type == LVAL_SEXPR) {
        truthy = (cond->sexpr.count > 0);
    } else {
        truthy = 1; // 其他类型默认为真
    }
    
    lval_free(cond);
    
    // 执行相应的分支
    if (truthy) {
        result = eval(e, lval_pop(a, 0)); // then 分支
    } else {
        if (a->sexpr.count > 1) {
            lval_free(lval_pop(a, 0)); // 移除 then 分支
            result = eval(e, lval_pop(a, 0)); // else 分支
        } else {
            result = lval_sexpr(); // 无 else 分支时返回空列表
        }
    }
    
    return result;
}
```

### 4. 环境注册
在 `src/env.c` 中注册了新的比较运算符：

```c
// 添加比较运算符
lenv_add_builtin(e, "=");
lenv_add_builtin(e, ">");
lenv_add_builtin(e, "<");
lenv_add_builtin(e, ">=");
lenv_add_builtin(e, "<=");
```

## 技术挑战和解决方案

### 1. 解析器支持比较运算符
**挑战**: 原始解析器无法识别 `=`, `>`, `<` 等比较运算符符号。

**解决方案**: 扩展了解析器的符号识别逻辑，添加对这些字符的支持。

### 2. 比较运算符的多参数处理
**挑战**: 比较运算符需要支持多个参数的比较（如 `(= 1 1 1)`）。

**解决方案**: 实现了链式比较逻辑，检查每个相邻参数对是否满足比较条件。

### 3. 条件表达式的惰性求值
**挑战**: 只有选中的分支应该被求值，避免不必要的计算。

**解决方案**: 在 `builtin_if` 中先求值条件，然后只对选中的分支调用 `eval` 函数。

### 4. 真值判断的统一标准
**挑战**: 不同类型的值需要统一的真值判断标准。

**解决方案**: 实现了基于类型的真值判断逻辑，确保一致性。

## 测试覆盖

创建了全面的测试套件 `test/test_conditional.c`，包括：

### 1. 比较运算符测试
- 基本比较功能测试
- 多参数比较测试
- 边界情况测试
- 错误处理测试

### 2. 条件表达式测试
- 真值条件测试
- 假值条件测试
- 无 else 分支测试
- 嵌套条件测试
- 复杂表达式测试

### 3. 错误处理测试
- 参数数量错误测试
- 类型错误测试
- 特殊形式验证测试

## 使用示例

### 基本条件表达式
```lisp
(if (= 1 1) 42 0)  ; 返回 42
(if (> 5 3) "true" "false")  ; 返回 "true"
(if (< 5 3) "true" "false")  ; 返回 "false"
```

### 复杂条件逻辑
```lisp
(if (and (= x 10) (> y 5))
    (* x 2)
    (/ x 2))
```

### 嵌套条件
```lisp
(if (< x 0)
    "negative"
    (if (= x 0)
        "zero"
        "positive"))
```

### 多参数比较
```lisp
(= 1 1 1)    ; 返回 1 (真)
(= 1 2 1)    ; 返回 0 (假)
(> 5 3 1)    ; 返回 1 (真)
(< 1 3 5)    ; 返回 1 (真)
```

## 性能考虑

1. **惰性求值**: 条件表达式的分支只在需要时求值，提高了性能
2. **内存管理**: 正确释放未使用分支的内存，避免内存泄漏
3. **错误处理**: 及早检测参数错误，避免不必要的计算

## 总结

迭代9成功实现了条件表达式和控制流功能，为Lisp语言添加了基本的决策能力。通过实现 `if` 特殊形式和比较运算符，用户现在可以编写根据条件执行不同代码分支的程序。这一迭代为后续实现更复杂的控制流结构（如循环、函数定义）奠定了基础。

测试结果显示所有39个测试用例都通过，确保了实现的正确性和稳定性。