[中文](#中文) | [English](#english)

---

<a id="中文"></a>

# skiplist / skiplist2 — C 跳表实现

同一个跳表算法的两种 C 实现方案。

---

## skiplist (v1) — 动态类型

动态类型跳表。使用 C11 `_Generic` + `union element_t` 在运行时统一处理多种类型。

**适用场景**：需要单一跳表实例处理多种数据类型，或不想在编译期固定类型。

→ [skiplist/README.md](skiplist/README.md)

## skiplist2 (v2) — 宏模板

宏模板跳表。使用 C 预处理器在编译期为每对 key/value 类型生成独立的跳表代码，单头文件即拷即用。

**适用场景**：对性能敏感，类型在编译期确定，愿意为每对类型组合生成独立代码。

→ [skiplist2/README.md](skiplist2/README.md)

## 对比

| | skiplist (v1) | skiplist2 (v2) |
|---|---|---|
| 类型方案 | union + `_Generic` 动态分发 | 宏模板编译期生成类型专用代码 |
| 类型检查 | 运行时（`NDEBUG` 控制） | 编译期 |
| 存储方式 | 所有 key/value 存为 `element_t` union | 直接存储原生 C 类型 |
| 调用方式 | 宏 → 直接函数调用 | 结构体内嵌函数指针（compare 为直接调用可内联） |
| 单实例多类型 | 支持 | 不支持 |
| 代码体积 | 一份代码处理所有类型 | 每对类型组合生成独立代码 |
| 扩展新类型 | 扩展 `element_t` union 即可 | 调用宏参数 |
| 语言标准 | 需 C11（`_Generic`） | 核心功能 C99，print 需 C11 |
| 编译方式 | 传统 .h + .c | 单头文件：`DECLARE_SKIP_LIST` + `DEF_SKIP_LIST` 两宏 |

## 性能基准

`uint32_t` key/value，1000 万元素，`-O3`，同一数据集，v1 开启 `NDEBUG`。
每个实现依次执行：插入全部 + 查找全部 + 删除全部，计时器包裹整体：

| 实现 | 总耗时（insert + find + remove） |
|------|---------------------------------|
| skiplist (v1) | 96.2s |
| skiplist2 (v2) | 88.8s **(-8%)** |

v2 的比较函数为编译期可见的直接调用（`compare_##KNAME##_##VNAME`），编译器在 `-O2`/`-O3` 下会自动内联，无需额外添加 `inline` 关键字。配合零装箱的原生类型存储，v2 性能已反超 v1。v1 未开启 `NDEBUG` 时，每次操作还需额外运行时类型检查，性能会进一步下降。

## License

参见 [LICENSE](LICENSE)。

---

<a id="english"></a>

# skiplist / skiplist2 — C Skiplist Implementations

Two C implementations of the same skiplist algorithm, exploring the space of generic programming in C.

---

## skiplist (v1) — Dynamic Typing

A dynamically-typed skiplist using C11 `_Generic` + `union element_t` to handle multiple types at runtime.

**Best for**: when you need one skiplist instance to handle multiple data types, or prefer not to commit to a type at compile time.

→ [skiplist/README.md](skiplist/README.md)

## skiplist2 (v2) — Macro Templates

A macro-template skiplist that generates type-specialized code at compile time via the C preprocessor. Single-header library — copy and use.

**Best for**: when performance matters, types are known at compile time, and you are willing to generate separate code per type pair.

→ [skiplist2/README.md](skiplist2/README.md)

## Comparison

| | skiplist (v1) | skiplist2 (v2) |
|---|---|---|
| Type scheme | union + `_Generic` dynamic dispatch | Macro templates generate type-specialized code |
| Type checking | Runtime (controlled by `NDEBUG`) | Compile time |
| Storage | All key/value stored as `element_t` union | Native C types stored directly |
| Call style | Macro → direct function call | Function pointers in struct (compare is direct, inlineable) |
| Multi-type per instance | Yes | No |
| Code size | One implementation for all types | Per-type-pair code generation |
| Adding new types | Extend `element_t` union | Invoke macro with new params |
| Language standard | Requires C11 (`_Generic`) | Core is C99, print requires C11 |
| Distribution | Traditional .h + .c | Single-header: `DECLARE_SKIP_LIST` + `DEF_SKIP_LIST` |

## Performance

`uint32_t` key/value, 10 million elements, `-O3`, same dataset, v1 with `NDEBUG`.
Each implementation runs: insert all + find all + remove all, timed as a whole:

| Implementation | Total time (insert + find + remove) |
|---------------|-------------------------------------|
| skiplist (v1) | 96.2s |
| skiplist2 (v2) | 88.8s **(-8%)** |

v2's compare function is a direct, compile-time-visible call (`compare_##KNAME##_##VNAME`); the compiler auto-inlines it at `-O2`/`-O3` — no `inline` keyword needed. Combined with zero-boxing native type storage, v2 now outperforms v1. When `NDEBUG` is not defined, v1 incurs additional runtime type checks, further widening the gap.

## License

See [LICENSE](LICENSE)。
