
##### skiplist 主要设计理念
1. 使用C语言实现.
2. 支持排名, 支持正序逆序遍历.
3. 支持多种数据类型, 最好能够像C++ stl一样使用.
4. 支持保存重复的key (按需), 这样能够实现一个多重字典, 以及后续更方便将skiplist作为其他工具的一个基础组件使用(例如实现内存数据库索引).
5. 实现了一个动态类型的skiplist, 提供一套根据参数数据类型来进行实际操作的宏 , 为了保证数据类型的一致性, 这些宏支持类型检查 (没有定义`NDEBUG` 宏时).
