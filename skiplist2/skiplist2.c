#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

// sed s/@/KNAME##_##VNAME/g


#define SKIPLIST_MAXLEVEL 32 /* Should be enough for 2^64 elements */
#define SKIPLIST_P 0.25      /* Skiplist P = 1/4 */



#define DEF_SKIP_NODE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
typedef struct skip_node_##@ skip_node_##@##_t; \
struct skip_node_##@ { \
    KEY_TYPE key; \
    VALUE_TYPE value; \
    skip_node_##@##_t *backward; \
    struct skiplist_level_##@##VNAME { \
        skip_node_##@##_t *forward; \
        unsigned long span;  \
    }level[]; \
};


#define DEF_SKIP_LIST(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_NODE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
typedef struct skip_list_##@ skip_list_##@##_t; \
typedef skip_node_##@##_t* (*insert_func_##@##_t)(skip_list_##@##_t *l, KEY_TYPE key, VALUE_TYPE value); \
typedef skip_node_##@##_t* (*find_func_##@##_t)(skip_list_##@##_t *l, KEY_TYPE key); \
typedef bool (*remove_func_##@##_t)(skip_list_##@##_t *l, KEY_TYPE key); \
typedef bool (*remove_node_func_##@##_t)(skip_list_##@##_t *l, skip_node_##@##_t *node); \
typedef unsigned long (*get_rank_func_##@##_t)(skip_list_##@##_t *l, KEY_TYPE key); \
typedef unsigned long (*get_node_rank_func_##@##_t)(skip_list_##@##_t *l, skip_node_##@##_t *node); \
typedef skip_node_##@##_t* (*get_node_by_rank_func_##@##_t)(skip_list_##@##_t *l, unsigned long rank); \
struct skip_list_##@ { \
    unsigned long length; \
    int level; \
    skip_node_##@##_t *header; \
    insert_func_##@##_t insert; \
    find_func_##@##_t find; \
    remove_func_##@##_t remove; \
    remove_node_func_##@##_t remove_node; \
    get_rank_func_##@##_t get_rank; \
    get_node_rank_func_##@##_t get_node_rank; \
    get_node_by_rank_func_##@##_t get_node_by_rank; \
};




#define DEF_SKIP_LIST_CREATE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_list_##@##_t* skip_list_create_##@(){ \
    skip_list_##@##_t *slist = malloc(sizeof(*slist)); \
    slist->level = 1; \
    slist->length = 0; \
    KEY_TYPE dummy_key; \
    VALUE_TYPE dummy_value; \
    skip_node_##KNAME## _##VNAME##_t *header = skip_node_create_##@(SKIPLIST_MAXLEVEL, dummy_key, dummy_value); \
    header->backward = header; \
    for(int i=0; i<SKIPLIST_MAXLEVEL; i++){ \
        header->level[i].forward = header; \
        header->level[i].span = 0; \
    } \
    slist->header = header; \
    slist->insert = &skip_list_insert_##@; \
    slist->insert_multi = &skip_list_insert_multi_##@; \
    slist->find = &skip_list_find_##@; \
    slist->remove = &skip_list_remove_##@; \
    slist->remove_node = &skip_list_remove_node_##@; \
    slist->get_rank = &skip_list_get_rank_##@; \
    slist->get_node_rank = &skip_list_get_node_rank_##@; \
    slist->get_node_by_rank = &skip_list_get_node_by_rank; \
    return slist; \
}


#define DEF_SKIP_LIST_INSERT(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_node_##@##_t *skip_list_insert_##@(skip_list_##@##_t *l, KEY_TYPE key, VALUE_TYPE value){ \
    skip_node_##@##_t *update[SKIPLIST_MAXLEVEL] = {}; \
    unsigned long rank[SKIPLIST_MAXLEVEL] = {}; \
    skip_node_##@##_t *cur = l->header; \
    for(int i=l->level-1; i>=0; i--){ \
        rank[i] = i == (l->level-1) ? 0 : rank[i+1]; \
        while(cur->level[i].forward != l->header){ \
            int comp = l->compare(cur->level[i].forward->key, key); \
            if(comp < 0){ \
                rank[i] += cur->level[i].span;\
                cur = cur->level[i].forward; \
            }else if(comp == 0){\
                return NULL; \
            }else { \
                break; \
            } \
        } \
        update[i] = cur; \
    } \
    int insert_level = random_level();\
    skip_node_##@##_t *node = skip_node_create_##@(insert_level, key, value); \
    if(insert_level > l->level){ \
        for(int i=l->level; i<insert_level; i++){ \
            rank[i] = 0; \
            update[i] = l->header; \
            update[i]->level[i].span = l->length; \
        } \
        l->level = insert_level; \
    } \
    for(int i=0; i<insert_level ; i++){ \
        node->level[i].forward = update[i]->level[i].forward; \
        skip_node_##@##_t *prev = update[i]; \
        prev->level[i].forward = node; \
        node->level[i].span = prev->level[i].span - (rank[0] - rank[i]); \
        prev->level[i].span = (rank[0] - rank[i])+1; \
    } \
    node->backward = update[0]; \
    node->level[0].forward->backward = node; \
    for(int i=insert_level; i < l->level; i++){ \
        update[i]->level[i].span++; \
    } \
    l->length++; \
    return node; \
}




// DEF_SKIP_NODE(char *, s, char *, s)


// DEF_SKIP_LIST_CREATE(char *, s, int32_t, int32)


DEF_SKIP_LIST(char *, s, int32_t, int32)

DEF_SKIP_LIST_INSERT(char *, s, int32_t, int32)
