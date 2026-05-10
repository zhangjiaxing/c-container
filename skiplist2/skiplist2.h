#ifndef SKIPLIST2_H
#define SKIPLIST2_H

#include <stdbool.h>
#include <string.h>

#define SKIPLIST_MAXLEVEL 32 /* Should be enough for 2^64 elements */
#define SKIPLIST_P 0.25      /* Skiplist P = 1/4 */



#define DECLARE_SKIP_NODE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
typedef struct skip_node_##KNAME##_##VNAME skip_node_##KNAME##_##VNAME##_t; \
struct skip_node_##KNAME##_##VNAME { \
    KEY_TYPE key; \
    VALUE_TYPE value; \
    skip_node_##KNAME##_##VNAME##_t *backward; \
    struct skiplist_level_##KNAME##_##VNAME { \
        skip_node_##KNAME##_##VNAME##_t *forward; \
        unsigned long span; \
    }level[]; \
};

#define DECLARE_SKIP_LIST_CREATE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_list_##KNAME##_##VNAME##_t* skip_list_create_##KNAME##_##VNAME(void);

#define DECLARE_SKIP_LIST_DESTROY(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
void skip_list_destroy_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l);

#define DECLARE_SKIP_LIST_INSERT(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_node_##KNAME##_##VNAME##_t *skip_list_insert_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key, VALUE_TYPE value);

#define DECLARE_SKIP_LIST_INSERT_MULTI(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_node_##KNAME##_##VNAME##_t *skip_list_insert_multi_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key, VALUE_TYPE value);

#define DECLARE_SKIP_LIST_FIND(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_node_##KNAME##_##VNAME##_t *skip_list_find_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key);

#define DECLARE_SKIP_LIST_REMOVE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
bool skip_list_remove_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key);

#define DECLARE_SKIP_LIST_REMOVE_NODE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
bool skip_list_remove_node_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, skip_node_##KNAME##_##VNAME##_t *node);

#define DECLARE_SKIP_LIST_GET_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
unsigned long skip_list_get_rank_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key);

#define DECLARE_SKIP_LIST_GET_NODE_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
unsigned long skip_list_get_node_rank_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, skip_node_##KNAME##_##VNAME##_t *node);

#define DECLARE_SKIP_LIST_GET_NODE_BY_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_node_##KNAME##_##VNAME##_t *skip_list_get_node_by_rank_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, unsigned long rank);

#define DECLARE_SKIP_LIST_PRINT(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
void skip_list_print_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l);


#define DECLARE_SKIP_LIST(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_NODE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
typedef struct skip_list_##KNAME##_##VNAME skip_list_##KNAME##_##VNAME##_t; \
typedef skip_node_##KNAME##_##VNAME##_t* (*insert_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key, VALUE_TYPE value); \
typedef skip_node_##KNAME##_##VNAME##_t* (*find_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key); \
typedef bool (*remove_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key); \
typedef bool (*remove_node_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, skip_node_##KNAME##_##VNAME##_t *node); \
typedef unsigned long (*get_rank_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key); \
typedef unsigned long (*get_node_rank_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, skip_node_##KNAME##_##VNAME##_t *node); \
typedef skip_node_##KNAME##_##VNAME##_t* (*get_node_by_rank_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, unsigned long rank); \
typedef void (*print_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l); \
struct skip_list_##KNAME##_##VNAME { \
    unsigned long length; \
    int level; \
    skip_node_##KNAME##_##VNAME##_t *header; \
    int (*compare)(KEY_TYPE a, KEY_TYPE b); \
    insert_func_##KNAME##_##VNAME##_t insert; \
    insert_func_##KNAME##_##VNAME##_t insert_multi; \
    find_func_##KNAME##_##VNAME##_t find; \
    remove_func_##KNAME##_##VNAME##_t remove; \
    remove_node_func_##KNAME##_##VNAME##_t remove_node; \
    get_rank_func_##KNAME##_##VNAME##_t get_rank; \
    get_node_rank_func_##KNAME##_##VNAME##_t get_node_rank; \
    get_node_by_rank_func_##KNAME##_##VNAME##_t get_node_by_rank; \
    print_func_##KNAME##_##VNAME##_t print; \
}; \
DECLARE_SKIP_LIST_CREATE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_LIST_DESTROY(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_LIST_INSERT(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_LIST_INSERT_MULTI(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_LIST_FIND(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_LIST_REMOVE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_LIST_REMOVE_NODE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_LIST_GET_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_LIST_GET_NODE_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_LIST_GET_NODE_BY_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DECLARE_SKIP_LIST_PRINT(KEY_TYPE, KNAME, VALUE_TYPE, VNAME)


/* ========== foreach 遍历宏 ==========
 * 所有 v2 跳表结构体字段名相同，因此用泛型宏即可，无需类型参数。
 * safe 版本需用户传入 tmp 变量（C99 无法在宏内声明类型特定的临时变量）。
 */
#define skip_list_foreach(node, l) \
    for ((node) = (l)->header->level[0].forward; (node) != (l)->header; (node) = (node)->level[0].forward)

#define skip_list_foreach_reverse(node, l) \
    for ((node) = (l)->header->backward; (node) != (l)->header; (node) = (node)->backward)

#define skip_list_foreach_safe(node, l, tmp) \
    for ((node) = (l)->header->level[0].forward, (tmp) = (node)->level[0].forward; \
         (node) != (l)->header; \
         (node) = (tmp), (tmp) = (node)->level[0].forward)

#define skip_list_foreach_reverse_safe(node, l, tmp) \
    for ((node) = (l)->header->backward, (tmp) = (node)->backward; \
         (node) != (l)->header; \
         (node) = (tmp), (tmp) = (node)->backward)

#endif //ifndef SKIPLIST2_H
