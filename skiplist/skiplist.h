#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>


typedef union element {
    int32_t i32;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
    char *s;
    void *p;
    double f;
} element_t;


typedef enum element_type {
    TINT32,
    TUINT32,
    TINT64,
    TUINT64,
    TSTR,
    TPTR, //从指针开始暂时不支持做key
    TDOUBLE, // float 传参时候会转换成double, 所以不支持float
    TUNKNOW
} element_type_t;


extern char* const element_typename_list[];


#define ELEMENT_TYPEID(x) _Generic((x), \
                            int32_t: TINT32, uint32_t: TUINT32, \
                            int64_t: TINT64, uint64_t: TUINT64, \
                            char*: TSTR, void *: TPTR, \
                            double: TDOUBLE, \
                            default: TUNKNOW )


#define ELEMENT_TYPEIDNAME(typeid) ((typeid) > TUNKNOW? element_typename_list[TUNKNOW] : element_typename_list[(typeid)])


typedef struct skip_node skip_node_t;
typedef struct skip_list skip_list_t;


typedef int32_t (*compare_func_t)(element_t key, element_t value);
typedef void (*print_func_t)(skip_list_t *l);


struct skip_node {
    element_t key; //skiplist按照key的大小顺序存放, 当key一样时候, 按照skip_node的内存中地址顺序存放, 这样给定skip_node指针时候, 可以删除很快.
    element_t value;

    skip_node_t *backward;
    struct skiplist_level {
        skip_node_t *forward;
        //span在节点中存放到forward节点的距离,header节点中span存放到第一个节点中的距离, level[0]最后一个节点的span应该为0
        //这样insert时候, 只需要计算backward节点和当前节点的span, 不需要计算forward节点的span. 这样可以不需要判断forward节点是否是NULL/header.
        unsigned long span; 
    }level[];
};


struct skip_list {
    unsigned long length;
    int level;
    skip_node_t *header; //循环列表, 方便逆序遍历

    element_type_t key_type;
    element_type_t value_type;

    compare_func_t compare;
};


#define skip_list_foreach(node, l) \
        for ((node) = (l)->header->level[0].forward; (node)!=(l)->header; (node)=(node)->level[0].forward)


#define skip_list_foreach_safe(node, l) \
        (node) = (l)->header->level[0].forward; \
        for (skip_node_t *tMp__=(node)->level[0].forward; (node)!=(l)->header; (node)=tMp__, tMp__=(node)->level[0].forward)


#define skip_list_foreach_reverse(node, l) \
        for ((node) = (l)->header->backward; node!=(l)->header; (node)=(node)->backward)


#define skip_list_foreach_reverse_safe(node, l) \
        (node) = (l)->header->backward; \
        for (skip_node_t *tMp__=(node)->backward; (node)!=(l)->header; (node)=tMp__, tMp__=(node)->backward)


extern const compare_func_t compare_func_list[TSTR+1];

skip_list_t* skip_list_create(element_type_t key_typeid, element_type_t value_typeid, compare_func_t compare);


#define SKIP_LIST_CREATE(KEY_TYPE, VALUE_TYPE) ({ \
    KEY_TYPE __key__; \
    VALUE_TYPE __value__; \
    (void) __key__; \
    (void) __value__; \
    element_type_t __key_type__ = ELEMENT_TYPEID(__key__); \
    element_type_t __value_type__ = ELEMENT_TYPEID(__value__); \
    if(__key_type__ > TSTR){ \
        fprintf(stderr, "%s: line %d key type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__key_type__)); \
        _Exit(1); \
    } \
    if(__value_type__ > TDOUBLE){ \
        fprintf(stderr, "%s: line %d value type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__value_type__)); \
        _Exit(1); \
    } \
    skip_list_create(__key_type__, __value_type__, compare_func_list[__key_type__]); \
})


#define SKIP_LIST_CREATE_CUSTOM(KEY_TYPE, VALUE_TYPE, compare_func) ({ \
    KEY_TYPE __key__; \
    VALUE_TYPE __value__; \
    (void) __key__; \
    (void) __value__; \
    element_type_t __key_type__ = ELEMENT_TYPEID(__key__); \
    element_type_t __value_type__ = ELEMENT_TYPEID(__value__); \
    if(__key_type__ != TPTR){ \
        fprintf(stderr, "%s: line %d key type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__key_type__)); \
        _Exit(1); \
    } \
    if(__value_type__ > TDOUBLE){ \
        fprintf(stderr, "%s: line %d value type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__value_type__)); \
        _Exit(1); \
    } \
    skip_list_create(__key_type__, __value_type__, compare_func); \
})


#ifndef NDEBUG

#define SKIP_LIST_INSERT(list, key, value) ({ \
    element_type_t __key_type__ = ELEMENT_TYPEID(key); \
    element_type_t __value_type__ = ELEMENT_TYPEID(value); \
    if(__key_type__ != (list)->key_type){ \
        fprintf(stderr, "%s: line %d key type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__key_type__)); \
        _Exit(1); \
    } \
    if(__value_type__ != (list)->value_type){ \
        fprintf(stderr, "%s: line %d value type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__value_type__)); \
        _Exit(1); \
    } \
    skip_list_insert((list), (element_t)(key), (element_t)(value)); \
})


#define SKIP_LIST_INSERT_MULTI(list, key, value) ({ \
    element_type_t __key_type__ = ELEMENT_TYPEID(key); \
    element_type_t __value_type__ = ELEMENT_TYPEID(value); \
    if(__key_type__ != (list)->key_type){ \
        fprintf(stderr, "%s: line %d key type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__key_type__)); \
        _Exit(1); \
    } \
    if(__value_type__ != (list)->value_type){ \
        fprintf(stderr, "%s: line %d value type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__value_type__)); \
        _Exit(1); \
    } \
    skip_list_insert_multi((list), (element_t)(key), (element_t)(value)); \
})


#define SKIP_LIST_FIND(list, key) ({ \
    element_type_t __key_type__ = ELEMENT_TYPEID(key); \
    if(__key_type__ != (list)->key_type){ \
        fprintf(stderr, "%s: line %d key type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__key_type__)); \
        _Exit(1); \
    } \
    skip_list_find((list), (element_t)key); \
})


#define SKIP_LIST_REMOVE(list, key) ({ \
    element_type_t __key_type__ = ELEMENT_TYPEID(key); \
    if(__key_type__ != (list)->key_type){ \
        fprintf(stderr, "%s: line %d key type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__key_type__)); \
        _Exit(1); \
    } \
    skip_list_remove((list), (element_t)key); \
})


#define SKIP_LIST_GET_RANK(list, key) ({ \
    element_type_t __key_type__ = ELEMENT_TYPEID(key); \
    if(__key_type__ != (list)->key_type){ \
        fprintf(stderr, "%s: line %d key type (%s) error\n", __func__, __LINE__, ELEMENT_TYPEIDNAME(__key_type__)); \
        _Exit(1); \
    } \
    skip_list_get_rank((list), (element_t)key); \
})

#else

#define SKIP_LIST_INSERT(list, key, value) (skip_list_insert((list), (element_t)(key), (element_t)(value)))


#define SKIP_LIST_INSERT_MULTI(list, key, value) (skip_list_insert_multi((list), (element_t)(key), (element_t)(value)))


#define SKIP_LIST_FIND(list, key) (skip_list_find((list), (element_t)key))


#define SKIP_LIST_REMOVE(list, key) (skip_list_remove((list), (element_t)key))


#define SKIP_LIST_GET_RANK(list, key) (skip_list_get_rank((list), (element_t)key))

#endif //NDEBUG


#define SKIP_LIST_DESTROY(list) do{ skip_list_destroy(list); (list)=NULL; } while(0)


#define SKIP_LIST_REMOVE_NODE(list, node) (skip_list_remove_node((list), node))


#define SKIP_LIST_GET_NODE_RANK(list, node) (skip_list_get_node_rank((list), node))


#define SKIP_LIST_GET_NODE_BY_RANK(list, rank) (skip_list_get_node_by_rank((list), (rank)))


void skip_list_print(skip_list_t *l);


void skip_list_rank_print(skip_list_t *l);


void skip_list_addr_print(skip_list_t *l);


void skip_list_destroy(skip_list_t *l);


skip_node_t *skip_list_insert(skip_list_t *l, element_t key, element_t value);


skip_node_t *skip_list_insert_multi(skip_list_t *l, element_t key, element_t value);


skip_node_t *skip_list_find(skip_list_t *l, element_t ele);


bool skip_list_remove(skip_list_t *l, element_t ele);


bool skip_list_remove_node(skip_list_t *l, skip_node_t *node);


unsigned long skip_list_get_rank(skip_list_t *l, element_t ele);


unsigned long skip_list_get_node_rank(skip_list_t *l, skip_node_t *node);


skip_node_t *skip_list_get_node_by_rank(skip_list_t *l, unsigned long rank);


#endif //ifndef SKIPLIST_H
