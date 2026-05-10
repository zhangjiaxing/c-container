# skiplist / skiplist2 — C 跳表实现

同一个跳表算法的两种 C 实现方案。

---

## skiplist (v1) — 动态类型

动态类型跳表。使用 C11 `_Generic` + `union element_t` 在运行时统一处理多种类型。

**适用场景**：需要单一跳表实例处理多种数据类型，或不想在编译期固定类型。

→ [skiplist/README.md](skiplist/README.md)

## skiplist2 (v2) — 宏模板

宏模板跳表。使用 C 预处理器在编译期为每对 key/value 类型生成独立的跳表代码，零运行时额外开销。

**适用场景**：对性能敏感，类型在编译期确定，愿意为每对类型组合生成独立代码。

→ [skiplist2/README.md](skiplist2/README.md)

## 对比

| | skiplist (v1) | skiplist2 (v2) |
|---|---|---|
| 类型方案 | union + `_Generic` 动态分发 | 宏模板编译期生成类型专用代码 |
| 类型检查 | 运行时（`NDEBUG` 控制） | 编译期 |
| 存储方式 | 所有 key/value 存为 `element_t` union | 直接存储原生 C 类型 |
| 零开销抽象 | 否（union 拷贝） | 是 |
| 单实例多类型 | 支持 | 不支持 |
| 代码体积 | 一份代码处理所有类型 | 每对类型组合生成独立代码 |
| 扩展新类型 | 扩展 `element_t` union 即可 | 调用宏参数 |
| 语言标准 | 需 C11（`_Generic`） | 核心功能 C99，print 需 C11 |
| 编译方式 | 传统 .h + .c | `DECLARE_SKIP_LIST` + `DEF_SKIP_LIST` 两宏 |

## License

参见 [LICENSE](LICENSE)。
