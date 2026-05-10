[中文](#中文) | [English](#english)

---

<a id="中文"></a>

# skiplist — 动态类型跳表

一个用 C 语言实现的跳表（skiplist），支持多种数据类型、排名查询、正序/逆序遍历。

---

### 设计理念

1. 使用 C11 `_Generic` + `union element_t` 实现动态类型。
2. 支持按排名查询，支持正序/逆序遍历。
3. 支持多种内置数据类型：`int32_t`、`uint32_t`、`int64_t`、`uint64_t`、`char*`、`void*`、`double`。
4. 支持重复 key（`INSERT_MULTI`），可用于实现多重字典或作为内存数据库索引的基础组件。
5. 通过宏封装提供类型检查（未定义 `NDEBUG` 时），保证传入类型一致性。

### 核心实现

- `element_t` 联合体统一存储不同类型的 key/value。
- `compare_func_t` 函数指针实现多态比较，系统预置 `int32_t` ~ `string` 的默认比较器。
- 跳表节点使用柔性数组成员（flexible array member）存储多层 forward 指针和 span。
- **循环链表设计**：header 节点的 backward 指向尾节点，方便逆序遍历。
- span 机制支持 `O(log N)` 的按排名查询。
- 节点按内存地址排序相同 key 的节点，支持通过指针快速删除。

### 文件结构

| 文件 | 说明 |
|------|------|
| `skiplist.h` | 类型定义、宏 API、函数声明 |
| `skiplist.c` | 核心算法实现 |
| `test.c` | 功能测试和基准测试 |
| `makefile` | 编译脚本 |

### API 一览

```c
// 创建跳表（默认比较器）
skip_list_t *list = SKIP_LIST_CREATE(key_type, value_type);

// 创建跳表（自定义比较器，key 须为 TPTR）
skip_list_t *list = SKIP_LIST_CREATE_CUSTOM(key_type, value_type, compare_func);

// 插入（key 已存在时返回 NULL）
SKIP_LIST_INSERT(list, key, value);

// 插入（允许重复 key）
SKIP_LIST_INSERT_MULTI(list, key, value);

// 查找
SKIP_LIST_FIND(list, key);

// 删除 by key
SKIP_LIST_REMOVE(list, key);

// 删除 by node 指针
SKIP_LIST_REMOVE_NODE(list, node);

// 按排名查询
SKIP_LIST_GET_NODE_BY_RANK(list, rank);
unsigned long rank = SKIP_LIST_GET_RANK(list, key);
unsigned long rank = SKIP_LIST_GET_NODE_RANK(list, node);

// 打印
skip_list_print(list);           // key(value)
skip_list_rank_print(list);      // key(span)
skip_list_addr_print(list);      // key(addr)

// 遍历
skip_list_foreach(node, list)             { /* key, value */ }
skip_list_foreach_safe(node, list)        { /* 可安全删除当前节点 */ }
skip_list_foreach_reverse(node, list)     { /* 逆序 */ }
skip_list_foreach_reverse_safe(node, list){ /* 逆序，可安全删除 */ }

// 销毁
SKIP_LIST_DESTROY(list);
```

### Debug 模式

定义 `NDEBUG` 宏关闭插入/查找/删除时的类型检查，获得更高性能。未定义 `NDEBUG` 时，所有宏操作会在运行时校验 key/value 类型是否匹配创建时声明的类型，类型不匹配会立即终止程序。

### 编译

```bash
make
```

### License

本项目代码可自由使用。

---

<a id="english"></a>

# skiplist — Dynamically-Typed Skiplist

A C skiplist with dynamic typing, rank queries, and bidirectional traversal.

---

### Design

1. Uses C11 `_Generic` + `union element_t` for runtime type polymorphism.
2. Supports rank queries and forward/reverse traversal.
3. Built-in types: `int32_t`, `uint32_t`, `int64_t`, `uint64_t`, `char*`, `void*`, `double`.
4. Duplicate keys via `INSERT_MULTI` — useful for multi-dicts or as an in-memory DB index primitive.
5. Macro-wrapped type checking (when `NDEBUG` is not defined) ensures type consistency.

### Implementation

- `element_t` union stores key/value of different types without per-type allocation.
- `compare_func_t` function pointers enable polymorphic comparison; default comparators for `int32_t` through `string` are built in.
- Nodes use flexible array members for multi-level forward pointers and spans.
- **Circular list**: header's `backward` points to the tail, enabling O(1) reverse traversal.
- span mechanism enables `O(log N)` rank queries.
- Same-key nodes ordered by memory address, enabling fast pointer-based deletion.

### Files

| File | Description |
|------|-------------|
| `skiplist.h` | Type definitions, macro API, function declarations |
| `skiplist.c` | Core algorithm implementation |
| `test.c` | Functional and benchmark tests |
| `makefile` | Build script |

### API

```c
// Create (default comparator)
skip_list_t *list = SKIP_LIST_CREATE(key_type, value_type);

// Create (custom comparator, key must be TPTR)
skip_list_t *list = SKIP_LIST_CREATE_CUSTOM(key_type, value_type, compare_func);

// Insert (returns NULL if key exists)
SKIP_LIST_INSERT(list, key, value);

// Insert (allows duplicate keys)
SKIP_LIST_INSERT_MULTI(list, key, value);

// Find
SKIP_LIST_FIND(list, key);

// Remove by key
SKIP_LIST_REMOVE(list, key);

// Remove by node pointer
SKIP_LIST_REMOVE_NODE(list, node);

// Rank queries
SKIP_LIST_GET_NODE_BY_RANK(list, rank);
unsigned long rank = SKIP_LIST_GET_RANK(list, key);
unsigned long rank = SKIP_LIST_GET_NODE_RANK(list, node);

// Print
skip_list_print(list);           // key(value)
skip_list_rank_print(list);      // key(span)
skip_list_addr_print(list);      // key(addr)

// Traversal
skip_list_foreach(node, list)             { /* key, value */ }
skip_list_foreach_safe(node, list)        { /* safe deletion */ }
skip_list_foreach_reverse(node, list)     { /* reverse */ }
skip_list_foreach_reverse_safe(node, list){ /* reverse, safe deletion */ }

// Destroy
SKIP_LIST_DESTROY(list);
```

### Debug Mode

Define `NDEBUG` to disable runtime type checks for maximum performance. Without `NDEBUG`, every macro operation validates that key/value types match those declared at creation time; mismatches abort the program immediately.

### Build

```bash
make
```

### License

This code is free to use.
