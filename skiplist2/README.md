# skiplist2 — 宏模板跳表

一个用 C 宏模板生成的跳表（skiplist）实现，在编译期为每对 key/value 类型组合生成类型专用的代码，无运行时类型开销。

---

### 设计理念

1. 使用 C 预处理器 `##` 标记粘贴（token pasting），为指定类型组合生成独立的跳表代码，类似 C++ template。
2. 节点直接存储原生 C 类型，无 union、无装箱、无间接访问，理论上零额外开销。
3. 核心操作通过结构体内嵌函数指针调用（`.insert()`、`.find()` 等），比较函数为编译期可见的直接调用（`compare_##KNAME##_##VNAME`），可被编译器内联。
4. **单头文件**：`skiplist2.h` 包含完整实现，`#include "skiplist2.h"` 后调用 `DECLARE_SKIP_LIST` + `DEF_SKIP_LIST` 两宏即可，零依赖。
5. 功能完整：支持唯一 key 插入、重复 key 插入（`insert_multi`）、按 key 查找/删除、按节点指针删除、按排名查询（`get_rank`/`get_node_rank`/`get_node_by_rank`）、销毁。打印函数（`DEF_SKIP_LIST_PRINT`）使用 `_Generic`，需 C11，不包含在 `DEF_SKIP_LIST` 中，用户按需单独调用。

### 核心实现

- 使用两条宏完成类型系统：
  - **`DECLARE_SKIP_LIST(KEY_TYPE, KNAME, VALUE_TYPE, VNAME)`** — 在头文件位置声明节点结构体、跳表结构体、函数指针类型、所有函数原型。
  - **`DEF_SKIP_LIST(KEY_TYPE, KNAME, VALUE_TYPE, VNAME)`** — 在源文件位置生成所有函数定义。
- 每个宏内部拆分为若干独立命名的子宏（如 `DECLARE_SKIP_LIST_FIND`、`DEF_SKIP_LIST_FIND`），提高可读性；组合宏 `DECLARE_SKIP_LIST` / `DEF_SKIP_LIST` 依次调用所有子宏。
- 比较函数命名约定：`compare_##KNAME##_##VNAME`，用户自行定义。各 DEF 宏通过该名称直接调用比较函数（非函数指针），编译器可内联。
- `_Generic` 实现 print 的自动格式化，无需传递格式字符串。print 依赖 C11，因此 `DEF_SKIP_LIST` 组合宏不包含 `DEF_SKIP_LIST_PRINT`，用户按需单独调用。
- 核心跳表功能（不含 print）兼容 C99。
- 循环链表设计：header 的 backward 指向尾节点。
- span 机制实现 `O(log N)` 按排名查询。
- 节点按内存地址排序相同 key 的节点，支持通过指针快速删除。

### 文件结构

| 文件 | 说明 |
|------|------|
| `skiplist2.h` | 完整库：声明宏 + 实现宏 + `random_level()`，单文件即拷即用 |
| `test.c` | 功能测试和基准测试（从 skiplist v1 移植） |
| `makefile` | 编译脚本，支持 C99 和 C11 两种目标 |

### 使用步骤

```c
#include "skiplist2.h"

// 1. 声明跳表类型和函数原型
DECLARE_SKIP_LIST(char *, s, int32_t, int32)

// 2. 定义比较函数（命名约定：compare_##KNAME##_##VNAME）
static int compare_s_int32(char *a, char *b) { return strcmp(a, b); }

// 3. 生成所有函数定义（print 不包含在内，如需使用请单独调用 DEF_SKIP_LIST_PRINT）
DEF_SKIP_LIST(char *, s, int32_t, int32)

// 4. 使用
void example() {
    skip_list_s_int32_t *list = skip_list_create_s_int32();
    list->insert(list, "key", 123);
    skip_node_s_int32_t *node = list->find(list, "key");
    unsigned long rank = list->get_rank(list, "key");
    list->remove(list, "key");
    skip_list_destroy_s_int32(list);
}
```

#### 参数说明

| 参数 | 含义 | 示例 |
|------|------|------|
| `KEY_TYPE` | key 的 C 类型 | `char *` |
| `KNAME` | key 类型的简短标识符 | `s`（对应 `char *`） |
| `VALUE_TYPE` | value 的 C 类型 | `int32_t` |
| `VNAME` | value 类型的简短标识符 | `int32` |

`KNAME` 和 `VNAME` 用于生成唯一的类型和函数名（如 `skip_list_s_int32_t`、`skip_list_insert_s_int32`）。

### 子宏一览

每个子宏对应一个独立的函数声明或定义，可直接调用（不依赖组合宏）：

| 子宏 | 声明位置 | 定义位置 |
|------|----------|----------|
| `DECLARE_SKIP_LIST_CREATE` / `DEF_SKIP_LIST_CREATE` | 创建跳表 | 头文件 |
| `DECLARE_SKIP_LIST_DESTROY` / `DEF_SKIP_LIST_DESTROY` | 销毁跳表 | 头文件 |
| `DECLARE_SKIP_LIST_INSERT` / `DEF_SKIP_LIST_INSERT` | 唯一 key 插入 | 头文件 |
| `DECLARE_SKIP_LIST_INSERT_MULTI` / `DEF_SKIP_LIST_INSERT_MULTI` | 重复 key 插入 | 头文件 |
| `DECLARE_SKIP_LIST_FIND` / `DEF_SKIP_LIST_FIND` | 按 key 查找 | 头文件 |
| `DECLARE_SKIP_LIST_REMOVE` / `DEF_SKIP_LIST_REMOVE` | 按 key 删除 | 头文件 |
| `DECLARE_SKIP_LIST_REMOVE_NODE` / `DEF_SKIP_LIST_REMOVE_NODE` | 按指针删除 | 头文件 |
| `DECLARE_SKIP_LIST_GET_RANK` / `DEF_SKIP_LIST_GET_RANK` | 按 key 查排名 | 头文件 |
| `DECLARE_SKIP_LIST_GET_NODE_RANK` / `DEF_SKIP_LIST_GET_NODE_RANK` | 按节点查排名 | 头文件 |
| `DECLARE_SKIP_LIST_GET_NODE_BY_RANK` / `DEF_SKIP_LIST_GET_NODE_BY_RANK` | 按排名查节点 | 头文件 |
| `DECLARE_SKIP_LIST_PRINT` / `DEF_SKIP_LIST_PRINT` | 打印跳表（需 C11 `_Generic`，`DEF_SKIP_LIST` 不包含） | 头文件 |

### foreach 遍历宏

所有 v2 跳表结构体字段名相同，因此遍历宏是泛型的，无需类型参数：

| 宏 | 说明 |
|----|------|
| `skip_list_foreach(node, list)` | 正序遍历，`node` 依次指向每个节点 |
| `skip_list_foreach_reverse(node, list)` | 逆序遍历 |
| `skip_list_foreach_safe(node, list, tmp)` | 正序遍历，可安全删除当前节点，需传入 `tmp` 临时变量 |
| `skip_list_foreach_reverse_safe(node, list, tmp)` | 逆序遍历，可安全删除当前节点 |

```c
// 正序遍历
skip_node_s_int32_t *node;
skip_list_foreach(node, list) {
    printf("%d\n", node->value);
}

// 逆序安全删除
skip_node_s_v_t *cur, *tmp;
skip_list_foreach_reverse_safe(cur, list, tmp) {
    if (strcmp(cur->key, "chrome") == 0)
        list->remove_node(list, cur);
}
```

以及两个内部辅助宏：
- `DECLARE_SKIP_NODE` — 声明节点结构体
- `DEF_SKIP_NODE_CREATE` / `DEF_SKIP_NODE_DESTROY` — 定义节点创建/销毁（static，仅供内部调用）

### 编译

```bash
make test      # C99 模式（不含 print）
make test-c11  # C11 模式（含 print）
```

`skiplist2` 是单头文件库，只需将 `skiplist2.h` 拷贝到项目中 `#include` 即可，零依赖。

一键编译并测试两种模式：

```bash
make
```

### 测试用例

测试从 skiplist v1 移植，覆盖：

| 测试函数 | 类型 | 内容 |
|----------|------|------|
| `test_basic` | `char*` → `int32_t` | insert 去重、find、get_rank、get_node_by_rank、remove、remove_node |
| `test_multi` | `char*` → `int32_t` | insert_multi 重复 key |
| `test_int32` | `int32_t` → `int32_t` | insert_multi + 唯一 key 混合、find、remove_node、get_node_rank |
| `test_uint32_bench` | `uint32_t` → `uint32_t` | 10M 元素插入基准测试 |
| `test_srt` | `char*` → `void*` | 字符串排序、逆序遍历、safe 遍历、get_rank、remove |
| `test_double` | `double` → `int32_t` | 浮点数 key 正常使用 |

### 与 skiplist (v1) 对比

| 特性 | skiplist (v1) | skiplist2 (v2) |
|------|---------------|----------------|
| 类型方案 | union `element_t` + `_Generic` 动态分发 | 宏模板编译期生成类型专用代码 |
| 类型检查 | 运行时（`NDEBUG` 控制） | 编译期（由 C 编译器保证） |
| 存储 | 所有 key/value 存为 `element_t` union | 直接存储原生 C 类型 |
| 性能 | union 拷贝 + 函数指针间接调用 | 零装箱开销，原生类型直存 |
| 代码体积 | 一份代码处理所有类型 | 每对类型组合生成独立代码 |
| 扩展性 | 扩展 `element_t` union 即可 | 需为每对类型组合调用宏 |
| 语言标准 | 需 C11（`_Generic`） | 核心功能 C99，print 需 C11 |
| 分发方式 | 传统 .h + .c | 单头文件（`skiplist2.h`），即拷即用 |
| 比较调用 | 函数指针（间接） | 直接调用（可内联） |
| 遍历方式 | 内置 foreach 宏 | `skip_list_foreach` / `skip_list_foreach_safe` / `skip_list_foreach_reverse` / `skip_list_foreach_reverse_safe` 宏 |

### License

本项目代码可自由使用。
