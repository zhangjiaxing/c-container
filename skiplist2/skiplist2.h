#ifndef SKIPLIST2_H
#define SKIPLIST2_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

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


/* =================================================================
 *  实现宏 — 始终定义，用户调用 DEF_SKIP_LIST(TYPE, KNAME, TYPE, VNAME)
 *  时展开为函数定义。
 * ================================================================*/

#define DEF_SKIP_NODE_CREATE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
static skip_node_##KNAME##_##VNAME##_t *skip_node_create_##KNAME##_##VNAME(int level, KEY_TYPE key, VALUE_TYPE value){ \
    skip_node_##KNAME##_##VNAME##_t *node = malloc(sizeof(*node) + level * sizeof(struct skiplist_level_##KNAME##_##VNAME)); \
    if (!node) return NULL; \
    node->key = key; \
    node->value = value; \
    return node; \
}

#define DEF_SKIP_NODE_DESTROY(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
static void skip_node_destroy_##KNAME##_##VNAME(skip_node_##KNAME##_##VNAME##_t *node){ \
    free(node); \
}

#define DEF_SKIP_LIST_CREATE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_list_##KNAME##_##VNAME##_t* skip_list_create_##KNAME##_##VNAME(){ \
    skip_list_##KNAME##_##VNAME##_t *slist = malloc(sizeof(*slist)); \
    if (!slist) return NULL; \
    slist->level = 1; \
    slist->length = 0; \
    KEY_TYPE dummy_key = (KEY_TYPE)0; \
    VALUE_TYPE dummy_value = (VALUE_TYPE)0; \
    skip_node_##KNAME##_##VNAME##_t *header = skip_node_create_##KNAME##_##VNAME(SKIPLIST_MAXLEVEL, dummy_key, dummy_value); \
    if (!header) { free(slist); return NULL; } \
    header->backward = header; \
    for(int i=0; i<SKIPLIST_MAXLEVEL; i++){ \
        header->level[i].forward = header; \
        header->level[i].span = 0; \
    } \
    slist->header = header; \
    slist->compare = compare_##KNAME##_##VNAME; \
    slist->insert = &skip_list_insert_##KNAME##_##VNAME; \
    slist->insert_multi = &skip_list_insert_multi_##KNAME##_##VNAME; \
    slist->find = &skip_list_find_##KNAME##_##VNAME; \
    slist->remove = &skip_list_remove_##KNAME##_##VNAME; \
    slist->remove_node = &skip_list_remove_node_##KNAME##_##VNAME; \
    slist->get_rank = &skip_list_get_rank_##KNAME##_##VNAME; \
    slist->get_node_rank = &skip_list_get_node_rank_##KNAME##_##VNAME; \
    slist->get_node_by_rank = &skip_list_get_node_by_rank_##KNAME##_##VNAME; \
    slist->print = NULL; \
    return slist; \
}

#define DEF_SKIP_LIST_DESTROY(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
void skip_list_destroy_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l){ \
    if (!l) return; \
    skip_node_##KNAME##_##VNAME##_t *cur = l->header->level[0].forward; \
    for(skip_node_##KNAME##_##VNAME##_t *next=cur->level[0].forward; cur!=l->header; cur=next, next=cur->level[0].forward){ \
        skip_node_destroy_##KNAME##_##VNAME(cur); \
    } \
    skip_node_destroy_##KNAME##_##VNAME(l->header); \
    free(l); \
}

#define DEF_SKIP_LIST_INSERT(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_node_##KNAME##_##VNAME##_t *skip_list_insert_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key, VALUE_TYPE value){ \
    skip_node_##KNAME##_##VNAME##_t *update[SKIPLIST_MAXLEVEL] = {}; \
    unsigned long rank[SKIPLIST_MAXLEVEL] = {}; \
    skip_node_##KNAME##_##VNAME##_t *cur = l->header; \
    for(int i=l->level-1; i>=0; i--){ \
        rank[i] = i == (l->level-1) ? 0 : rank[i+1]; \
        while(cur->level[i].forward != l->header){ \
            int comp = l->compare(cur->level[i].forward->key, key); \
            if(comp < 0){ \
                rank[i] += cur->level[i].span; \
                cur = cur->level[i].forward; \
            }else if(comp == 0){ \
                return NULL; \
            }else { \
                break; \
            } \
        } \
        update[i] = cur; \
    } \
    int insert_level = random_level(); \
    skip_node_##KNAME##_##VNAME##_t *node = skip_node_create_##KNAME##_##VNAME(insert_level, key, value); \
    if (!node) return NULL; \
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
        skip_node_##KNAME##_##VNAME##_t *prev = update[i]; \
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

#define DEF_SKIP_LIST_INSERT_MULTI(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_node_##KNAME##_##VNAME##_t *skip_list_insert_multi_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key, VALUE_TYPE value){ \
    skip_node_##KNAME##_##VNAME##_t *update[SKIPLIST_MAXLEVEL] = {}; \
    unsigned long rank[SKIPLIST_MAXLEVEL] = {}; \
    int insert_level = random_level(); \
    skip_node_##KNAME##_##VNAME##_t *node = skip_node_create_##KNAME##_##VNAME(insert_level, key, value); \
    if (!node) return NULL; \
    skip_node_##KNAME##_##VNAME##_t *cur = l->header; \
    for(int i=l->level-1; i>=0; i--){ \
        rank[i] = i == (l->level-1) ? 0 : rank[i+1]; \
        while(cur->level[i].forward != l->header){ \
            int comp = l->compare(cur->level[i].forward->key, key); \
            if(comp < 0 || (comp == 0 && cur->level[i].forward < node)){ \
                rank[i] += cur->level[i].span; \
                cur = cur->level[i].forward; \
            }else{ \
                break; \
            } \
        } \
        update[i] = cur; \
    } \
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
        skip_node_##KNAME##_##VNAME##_t *prev = update[i]; \
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

#define DEF_SKIP_LIST_FIND(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_node_##KNAME##_##VNAME##_t *skip_list_find_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key){ \
    if (!l) return NULL; \
    skip_node_##KNAME##_##VNAME##_t *cur = l->header; \
    for (int i = l->level-1; i >= 0; i--) { \
        while(cur->level[i].forward != l->header){ \
            int comp = l->compare(cur->level[i].forward->key, key); \
            if(comp < 0){ \
                cur = cur->level[i].forward; \
            }else { \
                break; \
            } \
        } \
    } \
    skip_node_##KNAME##_##VNAME##_t *next = cur->level[0].forward; \
    if(next != l->header && l->compare(next->key, key) == 0){ \
        return next; \
    }else{ \
       return NULL; \
    } \
}

#define DEF_SKIP_LIST_REMOVE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
bool skip_list_remove_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key){ \
    if (!l) return false; \
    skip_node_##KNAME##_##VNAME##_t *update[SKIPLIST_MAXLEVEL] = {}; \
    skip_node_##KNAME##_##VNAME##_t *cur = l->header; \
    for(int i=l->level-1; i>=0; i--){ \
        while(cur->level[i].forward != l->header){ \
            int comp = l->compare(cur->level[i].forward->key, key); \
            if(comp < 0){ \
                cur = cur->level[i].forward; \
            }else { \
                break; \
            } \
        } \
        update[i] = cur; \
    } \
    cur = cur->level[0].forward; \
    if(cur == l->header || l->compare(cur->key, key) != 0){ \
        return false; \
    } \
    for(int i=l->level-1; i>=0 ; i--){ \
        skip_node_##KNAME##_##VNAME##_t *prev = update[i]; \
        if(prev->level[i].forward == cur){ \
            prev->level[i].span += cur->level[i].span - 1; \
            prev->level[i].forward = cur->level[i].forward; \
        }else{ \
            prev->level[i].span--; \
        } \
    } \
    skip_node_##KNAME##_##VNAME##_t *next = cur->level[0].forward; \
    next->backward = update[0]; \
    skip_node_destroy_##KNAME##_##VNAME(cur); \
    l->length--; \
    while(l->level>1 && l->header->level[l->level-1].forward == l->header){ \
        l->level--; \
    } \
    return true; \
}

#define DEF_SKIP_LIST_REMOVE_NODE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
bool skip_list_remove_node_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, skip_node_##KNAME##_##VNAME##_t *node){ \
    if(!l || node == NULL || node == l->header){ \
        return false; \
    } \
    KEY_TYPE key = node->key; \
    skip_node_##KNAME##_##VNAME##_t *update[SKIPLIST_MAXLEVEL] = {}; \
    skip_node_##KNAME##_##VNAME##_t *cur = l->header; \
    for(int i=l->level-1; i>=0; i--){ \
        while(cur->level[i].forward != l->header){ \
            int comp = l->compare(cur->level[i].forward->key, key); \
            if(comp < 0 || (comp == 0 && cur->level[i].forward < node)){ \
                cur = cur->level[i].forward; \
            }else{ \
                break; \
            } \
        } \
        update[i] = cur; \
    } \
    cur = cur->level[0].forward; \
    if(cur == l->header || cur != node){ \
        return false; \
    } \
    skip_node_##KNAME##_##VNAME##_t *prev; \
    for(int i=l->level-1; i>=0 ; i--){ \
        prev = update[i]; \
        if(prev->level[i].forward == node){ \
            prev->level[i].span += cur->level[i].span - 1; \
            prev->level[i].forward = cur->level[i].forward; \
        }else{ \
            prev->level[i].span--; \
        } \
    } \
    skip_node_##KNAME##_##VNAME##_t *nxt = node->level[0].forward; \
    nxt->backward = update[0]; \
    skip_node_destroy_##KNAME##_##VNAME(node); \
    l->length--; \
    while(l->level>1 && l->header->level[l->level-1].forward == l->header){ \
        l->level--; \
    } \
    return true; \
}

#define DEF_SKIP_LIST_GET_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
unsigned long skip_list_get_rank_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, KEY_TYPE key){ \
    if (!l) return 0; \
    unsigned long rank = 0; \
    skip_node_##KNAME##_##VNAME##_t *cur = l->header; \
    for (int i = l->level-1; i >= 0; i--) { \
        while(cur->level[i].forward != l->header){ \
            int comp = l->compare(cur->level[i].forward->key, key); \
            if(comp < 0){ \
                rank += cur->level[i].span; \
                cur = cur->level[i].forward; \
            }else{ \
                break; \
            } \
        } \
    } \
    rank += cur->level[0].span; \
    cur = cur->level[0].forward; \
    if(cur != l->header && l->compare(cur->key, key) == 0){ \
        return rank; \
    }else{ \
        return 0; \
    } \
}

#define DEF_SKIP_LIST_GET_NODE_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
unsigned long skip_list_get_node_rank_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, skip_node_##KNAME##_##VNAME##_t *node){ \
    if(!l || node == NULL || node == l->header){ \
        return 0; \
    } \
    unsigned long rank = 0; \
    skip_node_##KNAME##_##VNAME##_t *cur = l->header; \
    for(int i = l->level-1; i >= 0; i--) { \
        while(cur->level[i].forward != l->header){ \
            int comp = l->compare(cur->level[i].forward->key, node->key); \
            if(comp < 0 || (comp == 0 && cur->level[i].forward <= node)){ \
                rank += cur->level[i].span; \
                cur = cur->level[i].forward; \
            }else{ \
                break; \
            } \
        } \
    } \
    if(cur == node){ \
        return rank; \
    }else{ \
        return 0; \
    } \
}

#define DEF_SKIP_LIST_GET_NODE_BY_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
skip_node_##KNAME##_##VNAME##_t *skip_list_get_node_by_rank_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l, unsigned long rank){ \
    if (!l) return NULL; \
    unsigned long traversed = 0; \
    skip_node_##KNAME##_##VNAME##_t *cur = l->header; \
    for (int i = l->level-1; i >= 0; i--) { \
        while (cur->level[i].forward != l->header && (traversed + cur->level[i].span) <= rank){ \
            traversed += cur->level[i].span; \
            cur = cur->level[i].forward; \
        } \
        if (traversed == rank){ \
            return cur; \
        } \
    } \
    return NULL; \
}

#define DEF_SKIP_LIST_PRINT(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
void skip_list_print_##KNAME##_##VNAME(skip_list_##KNAME##_##VNAME##_t *l){ \
    if (!l) return; \
    printf("list count: %lu, level is %d.\n", l->length, l->level); \
    for(int i=l->level-1; i>=0; i--){ \
        printf("level %d: ", i); \
        for(skip_node_##KNAME##_##VNAME##_t *cur=l->header->level[i].forward; cur!=l->header; cur=cur->level[i].forward){ \
            printf("("); \
            printf(_Generic((cur->key), \
                char *: "%s", \
                int: "%d", \
                unsigned: "%u", \
                long: "%ld", \
                unsigned long: "%lu", \
                long long: "%lld", \
                unsigned long long: "%llu", \
                double: "%f", \
                void *: "%p", \
                default: "?" \
            ), cur->key); \
            printf(","); \
            printf(_Generic((cur->value), \
                char *: "%s", \
                int: "%d", \
                unsigned: "%u", \
                long: "%ld", \
                unsigned long: "%lu", \
                long long: "%lld", \
                unsigned long long: "%llu", \
                double: "%f", \
                void *: "%p", \
                default: "?" \
            ), cur->value); \
            printf(")-"); \
        } \
        printf("NULL\n"); \
    } \
}

#define DEF_SKIP_LIST(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_NODE_CREATE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_NODE_DESTROY(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
\
DEF_SKIP_LIST_CREATE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_LIST_DESTROY(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_LIST_INSERT(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_LIST_INSERT_MULTI(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_LIST_FIND(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_LIST_REMOVE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_LIST_REMOVE_NODE(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_LIST_GET_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_LIST_GET_NODE_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME) \
DEF_SKIP_LIST_GET_NODE_BY_RANK(KEY_TYPE, KNAME, VALUE_TYPE, VNAME)

static int random_level(void) {
    static const int threshold = SKIPLIST_P * 0xFFFF;
    int level = 1;
    while ((rand() & 0xFFFF) < threshold)
        level += 1;
    return (level < SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL;
}

#endif /* SKIPLIST2_H */
