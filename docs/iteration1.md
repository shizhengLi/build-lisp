# 迭代1: 设置基本REPL框架



http://daiyuwen.freeshell.org/gb/rol/roots_of_lisp.html
https://www.paulgraham.com/avg.html
https://www.paulgraham.com/diff.html
https://www.paulgraham.com/lisplinks.html
https://www.paulgraham.com/lispart.html
https://www.paulgraham.com/apps.html
https://www.paulgraham.com/people.html
https://www.paulgraham.com/lispcode.html

## 目标
创建一个简单的命令行REPL，能读取输入、回显输出。

## 实现过程

### TDD流程

#### 1. Red - 编写失败的测试
首先编写了两个测试：
- `test_read_input()`: 测试输入"hello"期望输出"hello"
- `test_empty_input()`: 测试空输入期望返回NULL

#### 2. Green - 实现最小代码
实现了基本的REPL功能：
- `read_input()`: 处理输入并返回复制字符串
- `start_repl()`: 主REPL循环，读取用户输入并回显

#### 3. Refactor - 重构优化
- 添加了错误处理（空输入检查）
- 改善了内存管理（使用malloc/free）
- 分离了接口和实现

## 知识点总结

### 1. C语言基础
- 字符串处理：`strcpy`, `strlen`, `strcspn`
- 内存管理：`malloc`, `free`
- 输入输出：`fgets`, `printf`

### 2. TDD实践
- 红绿重构循环
- 单元测试框架MinUnit的使用
- 测试驱动的开发思维

### 3. 项目结构
- 模块化设计：分离头文件和实现文件
- Makefile编写：编译、测试、清理规则

## 踩坑记录

### 1. 缺少头文件包含
**问题**: 编译时报错，缺少`stdio.h`, `stdlib.h`, `string.h`等头文件
**解决**: 在测试文件中添加必要的头文件包含

### 2. 多重main函数定义
**问题**: 链接时报错，main函数被重复定义
**解决**: 修改Makefile，在构建测试时排除main.o

### 3. 字符串处理
**问题**: `fgets`读取的字符串包含换行符
**解决**: 使用`strcspn`函数去除换行符

### 4. 内存管理
**问题**: 忘记释放malloc分配的内存
**解决**: 添加对应的free调用，避免内存泄漏

## 测试结果
- 单元测试: 2个测试全部通过
- 集成测试: REPL能够正确回显输入
- 内存检查: 无内存泄漏

## 下一步
集成MPC解析器，实现基本的字符串解析功能。