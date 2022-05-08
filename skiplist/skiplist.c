/*
一个支持排名和多重key的skiplist
*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <errno.h>

#include "skiplist.h"


#define SKIPLIST_MAXLEVEL 32 /* Should be enough for 2^64 elements */
#define SKIPLIST_P 0.25      /* Skiplist P = 1/4 */


char* const element_typename_list[] = {
    "i32", "u32", "i64", "u64", "string", "pointer", "double", "unknow type"
};


// #define ELEMENT_TYPENAME(x) _Generic((x), \
//                             int32_t: "i32", uint32_t: "u32", \
//                             int64_t: "i64", uint64_t: "u64", \
//                             float: "float", double: "double", \
//                             char*: "string", void*: "pointer", \
//                             default: "unknow type" )


#define ELEMENT_TYPENAME(x) (element_typename_list[ELEMENT_TYPEID(x)])


//    printf(PRINTF_FMT "-", ele.FIELD);
#define DEF_ELEMENT_PRINT(TYPE, FIELD, PRINTF_FMT) \
static inline void print_element_##FIELD(element_t ele){ \
    printf(PRINTF_FMT, ele.FIELD); \
}


DEF_ELEMENT_PRINT(int32_t, i32, "%d")
DEF_ELEMENT_PRINT(uint32_t, u32, "%u")
DEF_ELEMENT_PRINT(int64_t, i64, "%ld")
DEF_ELEMENT_PRINT(uint64_t, u64, "%lu")
DEF_ELEMENT_PRINT(char*, s, "%s")
DEF_ELEMENT_PRINT(void*, p, "%p")
DEF_ELEMENT_PRINT(double, f, "%f")


#define DEF_SKIP_NODE_CREATE(KEY_TYPE, KEY_FIELD) \
skip_node_t *skip_node_create_ ## KEY_FIELD(int level, KEY_TYPE key, element_t value){ \
    skip_node_t *node = malloc(sizeof(*node) + level*(sizeof(struct skiplist_level))); \
    node->key.KEY_FIELD = key; \
    node->value = value; \
    return node; \
}


void skip_node_destroy(skip_node_t *node){
    free(node);
}

typedef void (*print_element_func_t)(element_t ele);


static const print_element_func_t print_element_func_list[TDOUBLE+1] = {
    &print_element_i32,
    &print_element_u32,
    &print_element_i64,
    &print_element_u64,
    &print_element_s,
    &print_element_p,
    &print_element_f
};


// #define DEF_SKIP_LIST_PRINT(KEY_TYPE, KEY_FIELD) \
// void skip_list_print_ ## KEY_FIELD(skip_list_t *l){ \
//     printf("\nskiplist: count %d\n", l->length); \
//     for(int i=l->level-1; i>=0; i--){ \
//         printf("level %d(span%lu): ", i,l->header->level[i].span); \
//         for(skip_node_t *cur=l->header->level[i].forward; cur!=l->header; cur=cur->level[i].forward){ \
//             print_element_##KEY_FIELD(cur->key); \
//             printf("(span%d)-", cur->level[i].span); \
//         } \
//         printf("NULL\n"); \
//     } \
// }


void skip_list_print(skip_list_t *l){
    printf("list count: %lu, level is %d.\n", l->length, l->level);
    for(int i=l->level-1; i>=0; i--){
        printf("level %d: ", i);
        for(skip_node_t *cur=l->header->level[i].forward; cur!=l->header; cur=cur->level[i].forward){
            print_element_func_list[l->key_type](cur->key);
            printf("(v");
            print_element_func_list[l->value_type](cur->value);
            printf(")-");
        }
        printf("NULL\n");
    }
}


void skip_list_rank_print(skip_list_t *l){
    printf("list count: %lu, level is %d.\n", l->length, l->level);
    for(int i=l->level-1; i>=0; i--){
        printf("level %d(span%lu): ", i,l->header->level[i].span);
        for(skip_node_t *cur=l->header->level[i].forward; cur!=l->header; cur=cur->level[i].forward){
            print_element_func_list[l->key_type](cur->key);
            printf("(span%lu)-", cur->level[i].span);
        }
        printf("NULL\n");
    }
}

void skip_list_addr_print(skip_list_t *l){
    printf("list count: %lu, level is %d.\n", l->length, l->level);
    for(int i=l->level-1; i>=0; i--){
        printf("level %d(%p): ", i, l->header);
        for(skip_node_t *cur=l->header->level[i].forward; cur!=l->header; cur=cur->level[i].forward){
            print_element_func_list[l->key_type](cur->key);
            printf("(addr%p)-", cur);
        }
        printf("NULL\n");
    }
}


#define DECLARE_SKIP_LIST_INSERT(KEY_TYPE, KEY_FIELD) \
skip_node_t *skip_list_insert_ ## KEY_FIELD(skip_list_t *l, element_t key, element_t value);


#define DECLARE_SKIP_LIST_INSERT_MULTI(KEY_TYPE, KEY_FIELD) \
skip_node_t *skip_list_insert_multi_ ## KEY_FIELD(skip_list_t *l, element_t key, element_t value);


#define DECLARE_SKIP_LIST_FIND(KEY_TYPE, KEY_FIELD) \
skip_node_t *skip_list_find_ ## KEY_FIELD(skip_list_t *l, element_t ele);


#define DECLARE_SKIP_LIST_REMOVE(KEY_TYPE, KEY_FIELD) \
bool skip_list_remove_ ## KEY_FIELD(skip_list_t *l, element_t ele);


#define DECLARE_SKIP_LIST_REMOVE_NODE(KEY_TYPE, KEY_FIELD) \
bool skip_list_remove_node_ ## KEY_FIELD(skip_list_t *l, skip_node_t *node);


#define DECLARE_SKIP_LIST_GET_RANK(KEY_TYPE, KEY_FIELD) \
unsigned long skip_list_get_rank_ ## KEY_FIELD(skip_list_t *l, element_t ele);


#define DECLARE_SKIP_LIST_GET_NODE_RANK(KEY_TYPE, KEY_FIELD) \
unsigned long skip_list_get_node_rank_ ## KEY_FIELD(skip_list_t *l, skip_node_t *node);


skip_node_t *skip_list_get_node_by_rank(skip_list_t *l, unsigned long rank);


#define DEF_SKIP_LIST_CREATE(KEY_TYPE, KEY_FIELD) \
skip_list_t* skip_list_create_ ## KEY_FIELD(element_type_t value_type_id){ \
    skip_list_t *slist = malloc(sizeof(*slist)); \
    slist->level = 1; \
    slist->length = 0; \
    skip_node_t *header = skip_node_create_ ## KEY_FIELD(SKIPLIST_MAXLEVEL, (KEY_TYPE)0, (element_t)0); \
    header->backward = header; \
    for(int i=0; i<SKIPLIST_MAXLEVEL; i++){ \
        header->level[i].forward = header; \
        header->level[i].span = 0; \
    } \
    slist->header = header; \
    slist->key_type = ELEMENT_TYPEID(header->key.KEY_FIELD); \
    slist->value_type = value_type_id; \
    slist->insert = &skip_list_insert_ ## KEY_FIELD; \
    slist->insert_multi = &skip_list_insert_multi_ ## KEY_FIELD; \
    slist->find = &skip_list_find_ ## KEY_FIELD; \
    slist->remove = &skip_list_remove_ ## KEY_FIELD; \
    slist->remove_node = &skip_list_remove_node_ ## KEY_FIELD; \
    slist->get_rank = &skip_list_get_rank_ ## KEY_FIELD; \
    slist->get_node_rank = &skip_list_get_node_rank_ ## KEY_FIELD; \
    slist->get_node_by_rank = &skip_list_get_node_by_rank; \
    return slist; \
} \


void skip_list_destroy(skip_list_t *l){
    skip_node_t *cur = l->header->level[0].forward;
    for(skip_node_t *next=cur->level[0].forward; cur!=l->header; cur=next, next=cur->level[0].forward){
        skip_node_destroy(cur);
    }
    skip_node_destroy(l->header);
    free(l);
}

static int random_level(void) {
    static const int threshold = SKIPLIST_P*RAND_MAX;
    int level = 1;
    while (rand() < threshold)
        level += 1;
    return (level<SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL;
}


static inline int element_compare_i32(int32_t e1, int32_t e2){
    return e1>e2 ? 1 : (e1==e2 ? 0 : -1);
}

static inline int element_compare_u32(uint32_t e1, uint32_t e2){
    return e1>e2 ? 1 : (e1==e2 ? 0 : -1);
}

static inline int element_compare_i64(int64_t e1, int64_t e2){
    return e1>e2 ? 1 : (e1==e2 ? 0 : -1);
}

static inline int element_compare_u64(uint64_t e1, uint64_t e2){
    return e1>e2 ? 1 : (e1==e2 ? 0 : -1);
}

static inline int element_compare_s(char *s1, char *s2){
    return strcmp(s1, s2);
}


#define DEF_SKIP_LIST_INSERT(KEY_TYPE, KEY_FIELD) \
skip_node_t *skip_list_insert_ ## KEY_FIELD(skip_list_t *l, element_t key, element_t value){ \
    skip_node_t *update[SKIPLIST_MAXLEVEL] = {}; \
    unsigned long rank[SKIPLIST_MAXLEVEL] = {}; \
    skip_node_t *cur = l->header; \
    for(int i=l->level-1; i>=0; i--){ \
        rank[i] = i == (l->level-1) ? 0 : rank[i+1]; \
        while(cur->level[i].forward != l->header){ \
            int comp = element_compare_##KEY_FIELD(cur->level[i].forward->key.KEY_FIELD , key.KEY_FIELD); \
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
    skip_node_t *node = skip_node_create_ ## KEY_FIELD(insert_level, key.KEY_FIELD, value); \
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
        skip_node_t *prev = update[i]; \
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


#define DEF_SKIP_LIST_INSERT_MULTI(KEY_TYPE, KEY_FIELD) \
skip_node_t *skip_list_insert_multi_ ## KEY_FIELD(skip_list_t *l, element_t key, element_t value){ \
    skip_node_t *update[SKIPLIST_MAXLEVEL] = {}; \
    unsigned long rank[SKIPLIST_MAXLEVEL] = {}; \
    int insert_level = random_level();\
    skip_node_t *node = skip_node_create_ ## KEY_FIELD(insert_level, key.KEY_FIELD, value); \
    skip_node_t *cur = l->header; \
    for(int i=l->level-1; i>=0; i--){ \
        rank[i] = i == (l->level-1) ? 0 : rank[i+1]; \
        while(cur->level[i].forward != l->header){ \
            int comp = element_compare_##KEY_FIELD(cur->level[i].forward->key.KEY_FIELD , key.KEY_FIELD); \
            if(comp < 0 || (comp == 0 && cur->level[i].forward < node)){ \
                rank[i] += cur->level[i].span;\
                cur = cur->level[i].forward; \
            }else{\
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
        skip_node_t *prev = update[i]; \
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



#define DEF_SKIP_LIST_FIND(KEY_TYPE, KEY_FIELD) \
skip_node_t *skip_list_find_ ## KEY_FIELD(skip_list_t *l, element_t ele){ \
    KEY_TYPE key = ele.KEY_FIELD;  \
    skip_node_t *cur = l->header; \
    for (int i = l->level-1; i >= 0; i--) { \
        while(cur->level[i].forward != l->header){ \
            int comp = element_compare_##KEY_FIELD(cur->level[i].forward->key.KEY_FIELD, key); \
            if(comp < 0){ \
                cur = cur->level[i].forward; \
            }else { \
                break; \
            } \
        } \
    } \
    skip_node_t *next = cur->level[0].forward; \
    if(next != l->header && element_compare_##KEY_FIELD(next->key.KEY_FIELD, key) == 0){ \
        return next; \
    }else{ \
       return NULL; \
    } \
} \


#define DEF_SKIP_LIST_REMOVE(KEY_TYPE, KEY_FIELD) \
bool skip_list_remove_ ## KEY_FIELD(skip_list_t *l, element_t ele){ \
    KEY_TYPE key = ele.KEY_FIELD;  \
    skip_node_t *update[SKIPLIST_MAXLEVEL] = {}; \
    skip_node_t *cur = l->header; \
    for(int i=l->level-1; i>=0; i--){ \
        while(cur->level[i].forward != l->header){ \
            int comp = element_compare_##KEY_FIELD(cur->level[i].forward->key.KEY_FIELD, key); \
            if(comp < 0){ \
                cur = cur->level[i].forward; \
            }else { \
                break; \
            } \
        } \
        update[i] = cur; \
    } \
    cur = cur->level[0].forward; \
    if(cur == l->header || element_compare_##KEY_FIELD(cur->key.KEY_FIELD, key) != 0){ \
        return false;\
    } \
    for(int i=l->level-1; i>=0 ; i--){ \
        skip_node_t *prev = update[i]; \
        if(prev->level[i].forward == cur){ \
            prev->level[i].span  += cur->level[i].span - 1; \
            prev->level[i].forward = cur->level[i].forward; \
        }else{ \
            prev->level[i].span --; \
        } \
    } \
    skip_node_t *next = cur->level[0].forward; \
    next->backward = update[0]; \
    skip_node_destroy(cur); \
    l->length--; \
    while(l->level>1 && l->header->level[l->level-1].forward == l->header){ \
        l->level--; \
    } \
    return true; \
}


#define DEF_SKIP_LIST_REMOVE_NODE(KEY_TYPE, KEY_FIELD) \
bool skip_list_remove_node_ ## KEY_FIELD(skip_list_t *l, skip_node_t *node){ \
    if(node == NULL || node == l->header){ \
        return false; \
    } \
    element_t ele = node->key; \
    skip_node_t *update[SKIPLIST_MAXLEVEL] = {}; \
    skip_node_t *cur = l->header; \
    for(int i=l->level-1; i>=0; i--){ \
        while(cur->level[i].forward != l->header){ \
            int comp = element_compare_##KEY_FIELD(cur->level[i].forward->key.KEY_FIELD , ele.KEY_FIELD); \
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
    skip_node_t *prev; \
    for(int i=l->level-1; i>=0 ; i--){ \
        prev = update[i]; \
        if(prev->level[i].forward == node){ \
            prev->level[i].span  += cur->level[i].span - 1; \
            prev->level[i].forward = cur->level[i].forward; \
        }else{ \
            prev->level[i].span--; \
        } \
    } \
    skip_node_t *next = node->level[0].forward; \
    next->backward = update[0]; \
    skip_node_destroy(node); \
    l->length--; \
    while(l->level>1 && l->header->level[l->level-1].forward == l->header){ \
        l->level--; \
    } \
    return true; \
}


#define DEF_SKIP_LIST_GET_RANK(KEY_TYPE, KEY_FIELD) \
unsigned long skip_list_get_rank_ ## KEY_FIELD(skip_list_t *l, element_t ele){ \
    unsigned long rank = 0; \
    skip_node_t *cur = l->header; \
    for (int i = l->level-1; i >= 0; i--) { \
        while(cur->level[i].forward != l->header){ \
            int comp = element_compare_##KEY_FIELD(cur->level[i].forward->key.KEY_FIELD , ele.KEY_FIELD); \
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
    if(cur != l->header && element_compare_##KEY_FIELD(cur->key.KEY_FIELD, ele.KEY_FIELD) == 0){ \
        return rank; \
    }else{ \
        return 0; \
    } \
}


#define DEF_SKIP_LIST_GET_NODE_RANK(KEY_TYPE, KEY_FIELD) \
unsigned long skip_list_get_node_rank_ ## KEY_FIELD(skip_list_t *l, skip_node_t *node){ \
    if(node == NULL || node == l->header){ \
        return ENOENT; \
    } \
    unsigned long rank = 0; \
    skip_node_t *cur = l->header; \
    for(int i = l->level-1; i >= 0; i--) { \
        while(cur->level[i].forward != l->header){ \
            int comp = element_compare_##KEY_FIELD(cur->level[i].forward->key.KEY_FIELD , node->key.KEY_FIELD); \
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


skip_node_t *skip_list_get_node_by_rank(skip_list_t *l, unsigned long rank){
    unsigned long traversed = 0;
    skip_node_t *cur = l->header;

    for (int i = l->level-1; i >= 0; i--) {
        while (cur->level[i].forward && (traversed + cur->level[i].span) <= rank){
            traversed += cur->level[i].span;
            cur = cur->level[i].forward;
        }
        if (traversed == rank){
            return cur;
        }
    }
    return NULL;
}


#define DEF_SKIP_LIST(KEY_TYPE, KEY_FIELD) \
    DECLARE_SKIP_LIST_INSERT(KEY_TYPE, KEY_FIELD) \
    DECLARE_SKIP_LIST_INSERT_MULTI(KEY_TYPE, KEY_FIELD) \
    DECLARE_SKIP_LIST_FIND(KEY_TYPE, KEY_FIELD) \
    DECLARE_SKIP_LIST_REMOVE(KEY_TYPE, KEY_FIELD) \
    DECLARE_SKIP_LIST_REMOVE_NODE(KEY_TYPE, KEY_FIELD) \
    DECLARE_SKIP_LIST_GET_RANK(KEY_TYPE, KEY_FIELD) \
    DECLARE_SKIP_LIST_GET_NODE_RANK(KEY_TYPE, KEY_FIELD) \
    DEF_SKIP_NODE_CREATE(KEY_TYPE, KEY_FIELD) \
    DEF_SKIP_LIST_INSERT(KEY_TYPE, KEY_FIELD) \
    DEF_SKIP_LIST_INSERT_MULTI(KEY_TYPE, KEY_FIELD) \
    DEF_SKIP_LIST_REMOVE(KEY_TYPE, KEY_FIELD) \
    DEF_SKIP_LIST_REMOVE_NODE(KEY_TYPE, KEY_FIELD) \
    DEF_SKIP_LIST_CREATE(KEY_TYPE, KEY_FIELD) \
    DEF_SKIP_LIST_FIND(KEY_TYPE, KEY_FIELD) \
    DEF_SKIP_LIST_GET_RANK(KEY_TYPE, KEY_FIELD) \
    DEF_SKIP_LIST_GET_NODE_RANK(KEY_TYPE, KEY_FIELD)


DEF_SKIP_LIST(int32_t, i32)
DEF_SKIP_LIST(uint32_t, u32)
DEF_SKIP_LIST(int64_t, i64)
DEF_SKIP_LIST(uint64_t, u64)
DEF_SKIP_LIST(char *, s)



const skip_list_create_func_t create_func_list[TSTR+1] = {
    skip_list_create_i32,
    skip_list_create_u32,
    skip_list_create_i64,
    skip_list_create_u64,
    skip_list_create_s
};
