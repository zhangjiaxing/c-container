
##### skiplist 主要设计理念
1. 使用C语言实现.
2. 支持多种数据类型, 最好能够像C++ stl一样使用.
3. 支持保存重复的key (按需), 这样能够实现一个多重字典, 以及后续更方便将skiplist作为其他工具的一个基础组件使用(例如实现内存数据库索引).
4. 高性能, 尽量避免比较函数带来的函数调用开销, 尽可能用inline函数实现比较.
5. 在保持高性能和支持多种数据类型的前提下, 使用比其他skiplist更方便. `void * + memcpy + compare函数`式的skiplist虽然能够实现支持不同数据类型, 但是性能差, 使用也不方便.
6. 由于C没有模板, 为了保证数据类型的一致性, 支持运行时类型检查 (没有定义`NDEBUG` 宏时).

