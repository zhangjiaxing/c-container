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
| 调用方式 | 宏 → 直接函数调用 | 结构体内嵌函数指针（间接调用） |
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
| skiplist (v1) | 95.9s |
| skiplist2 (v2) | 98.1s **(+2%)** |

v2 通过函数指针间接调用（`list->insert`），编译器无法内联，有少量开销。但整体差距在实际使用中基本可以忽略。v1 未开启 `NDEBUG` 时，每次操作还需额外运行时类型检查，性能会进一步下降。

## License

参见 [LICENSE](LICENSE)。
