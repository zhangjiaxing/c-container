#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



#define DEF_SKIP_NODE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
typedef struct skip_node_##KNAME##_##VNAME skip_node_##KNAME## _##VNAME##_t; \
struct skip_node_##KNAME##_##VNAME { \
    KEY_TYPE key; \
    VALUE_TYPE value; \
    skip_node_##KNAME##_##VNAME##_t *backward; \
    struct skiplist_level_##KNAME##_##VNAME { \
        skip_node_##KNAME##_##VNAME##_t *forward; \
        unsigned long span;  \
    }level[]; \
};



DEF_SKIP_NODE(char *, s, char *, s)


#define DEF_SKIP_LIST(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_NODE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
typedef struct skip_list_##KNAME##_##VNAME skip_list_##KNAME##_##VNAME##_t; \
typedef skip_node_##KNAME##_##VNAME##_t* (*insert_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key, VALUE_TYPE value); \
typedef skip_node_##KNAME##_##VNAME##_t* (*find_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key); \
typedef bool (*remove_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key); \
typedef bool (*remove_node_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, skip_node_##KNAME##_##VNAME##_t *node); \
typedef unsigned long (*get_rank_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key); \
typedef unsigned long (*get_node_rank_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, skip_node_##KNAME##_##VNAME##_t *node); \
typedef skip_node_##KNAME##_##VNAME##_t* (*get_node_by_rank_func_##KNAME##_##VNAME##_t)(skip_list_##KNAME##_##VNAME##_t *l, unsigned long rank); \
struct skip_list##KNAME##_##VNAME { \
    unsigned long length; \
    int level; \
    skip_node_##KNAME##_##VNAME##_t *header; \
    insert_func_##KNAME##_##VNAME##_t insert; \
    find_func_##KNAME##_##VNAME##_t find; \
    remove_func_##KNAME##_##VNAME##_t remove; \
    remove_node_func_##KNAME##_##VNAME##_t remove_node; \
    get_rank_func_##KNAME##_##VNAME##_t get_rank; \
    get_node_rank_func_##KNAME##_##VNAME##_t get_node_rank; \
    get_node_by_rank_func_##KNAME##_##VNAME##_t get_node_by_rank; \
};


DEF_SKIP_LIST(char *, s, int32_t, int32)

