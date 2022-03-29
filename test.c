#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>


#define SKIPLIST_MAXLEVEL 32 /* Should be enough for 2^64 elements */
#define SKIPLIST_P 0.25      /* Skiplist P = 1/4 */


typedef enum element_type {
    TINT32,
    TUINT32,
    TINT64,
    TUINT64,
    TFLOAT,
    TDOUBLE,
    TSTR,
    TPTR,
    TUNKNOW
} element_type_t;


char* const element_typename_list[] = {
    "i32", "u32", "i64", "u64", "float", "double", "string", "pointer", "unknow type"
};


typedef union element {
    int32_t i32;
    uint32_t u32;
    int64_t i64;
    uint64_t u64;
    float f;
    double d;
    char *s;
    void *p;
} element_t;



#define ELEMENT_TYPEID(x) _Generic((x), \
                            int32_t: TINT32, uint32_t: TUINT32, \
                            int64_t: TINT64, uint64_t: TUINT64, \
                            float: TFLOAT, double: TDOUBLE, \
                            char*: TSTR, void *: TPTR, \
                            default: TUNKNOW )

#define ELEMENT_TYPENAME(x) (element_typename_list[ELEMENT_TYPEID(x)])


// #define ELEMENT_TYPENAME(x) _Generic((x), \
//                             int32_t: "i32", uint32_t: "u32", \
//                             int64_t: "i64", uint64_t: "u64", \
//                             float: "float", double: "double", \
//                             char*: "string", void*: "pointer", \
//                             default: "unknow type" )



typedef struct skip_node skip_node_t;
typedef struct skip_list skip_list_t;


typedef skip_node_t* (*insert_func_t)(skip_list_t *l, element_t key, element_t value);
typedef element_t (*remove_func_t)(skip_list_t *l, element_t key);
typedef skip_node_t* (*find_func_t)(skip_list_t *l, element_t key);
typedef void (*print_func_t)(skip_list_t *l);



struct skip_node {
    element_t key;
    element_t value;
    skip_node_t *backward;
    skip_node_t *level[];
};

struct skip_list {
    unsigned long length;
    int level;
    skip_node_t *header;
    skip_node_t *tail;

    element_type_t key_type;
    insert_func_t insert_func;
    remove_func_t remove_func;
    find_func_t find_func;
    print_func_t print_func;
};

static int random_level(void) {
    static const int threshold = SKIPLIST_P*RAND_MAX;
    int level = 1;
    while (random() < threshold)
        level += 1;
    return (level<SKIPLIST_MAXLEVEL) ? level : SKIPLIST_MAXLEVEL;
}

#define DEF_ELEMENT_PRINT(TYPE, FIELD, PRINTF_FMT) \
void print_element_##FIELD(element_t ele){ \
    printf(PRINTF_FMT "-", ele.FIELD); \
}


DEF_ELEMENT_PRINT(char*, s, "%s")
DEF_ELEMENT_PRINT(uint32_t, u32, "%lu")
DEF_ELEMENT_PRINT(int32_t, i32, "%d")
DEF_ELEMENT_PRINT(void*, p, "%p")

#define DEF_SKIP_NODE_CREATE(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD) \
skip_node_t *skip_node_create_ ## KEY_FIELD ## _ ## VALUE_FIELD(int level, KEY_TYPE key, VALUE_TYPE value){ \
    skip_node_t *node = malloc(sizeof(*node) + level*(sizeof(skip_node_t *))); \
    node->key.KEY_FIELD = key; \
    node->value.VALUE_FIELD = value; \
    return node; \
}


#define DEF_SKIP_LIST_PRINT(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD) \
void skip_list_print_ ## KEY_FIELD ## _ ## VALUE_FIELD(skip_list_t *l){ \
    printf("\nskiplist: count %d\n", l->length); \
    for(int i=l->level-1; i>=0; i--){ \
        printf("level %d: ", i); \
        for(skip_node_t *cur=l->header->level[i]; cur!=NULL; cur=cur->level[i]){ \
            print_element_##KEY_FIELD(cur->key); \
        } \
        printf("NULL\n"); \
    } \
}


static inline int element_compare_i32(int32_t e1, int32_t e2){
    return e1 - e2;
}

static inline int element_compare_u32(uint32_t e1, uint32_t e2){
    return e1 - e2;
}

static inline int element_compare_i64(int64_t e1, int64_t e2){
    return e1 - e2;
}

static inline int element_compare_u64(uint64_t e1, uint64_t e2){
    return e1 - e2;
}

static inline int element_compare_s(char *s1, char *s2){
    return strcmp(s1, s2);
}

#define DEF_SKIP_LIST_INSERT(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD) \
skip_node_t *skip_list_insert_ ## KEY_FIELD ## _ ## VALUE_FIELD(skip_list_t *l, element_t key, element_t value){ \
    skip_node_t *update[SKIPLIST_MAXLEVEL] = {}; \
    skip_node_t *cur = l->header; \
    for(int i=l->level-1; i>=0; i--){ \
        while(cur->level[i] && element_compare_##KEY_FIELD(cur->level[i]->key.KEY_FIELD , key.KEY_FIELD) < 0 ){ \
            cur = cur->level[i]; \
        } \
        update[i] = cur; \
    }\
    int insert_level = random_level(); \
    if(insert_level > l->level){ \
        for(int i=l->level; i<insert_level; i++){ \
            update[i] = l->header; \
        } \
        l->level = insert_level; \
    } \
    skip_node_t *node = skip_node_create_ ## KEY_FIELD ## _ ## VALUE_FIELD(insert_level, key.KEY_FIELD, value.VALUE_FIELD); \
    for(int i=0; i<insert_level ; i++){ \
        skip_node_t *next = update[i]->level[i]; \
        node->level[i] = next; \
        skip_node_t *prev = update[i]; \
        prev->level[i] = node; \
    } \
    node->backward = update[0]; \
    if(node->level[0]){ \
        node->level[0]->backward = node; \
    }else{ \
        l->tail = node; \
    } \
    l->length++; \
    return node; \
}

#define DEF_SKIP_LIST_CREATE(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD) \
skip_list_t* skip_list_create_ ## KEY_FIELD ## _ ## VALUE_FIELD(){ \
    skip_list_t *slist = malloc(sizeof(*slist)); \
    slist->print_func = &skip_list_print_ ## KEY_FIELD ## _ ## VALUE_FIELD; \
    slist->insert_func = &skip_list_insert_ ## KEY_FIELD ## _ ## VALUE_FIELD; \
    slist->level = 1; \
    slist->length = 0; \
    slist->header = skip_node_create_ ## KEY_FIELD ## _ ## VALUE_FIELD(SKIPLIST_MAXLEVEL, (KEY_TYPE)0, (VALUE_TYPE)0); \
    slist->header->backward = NULL; \
    for(int i=0; i<SKIPLIST_MAXLEVEL; i++){ \
        slist->header->level[i] = NULL; \
    } \
    slist->tail = NULL; \
    return slist; \
}

#define DEF_SKIP_LIST(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD) \
    DEF_SKIP_NODE_CREATE(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD) \
    DEF_SKIP_LIST_PRINT(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD) \
    DEF_SKIP_LIST_INSERT(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD) \
    DEF_SKIP_LIST_CREATE(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD)


#define DECLARE_SKIP_NODE_CREATE(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD) \
    skip_node_t *skip_node_create_ ## KEY_FIELD ## _ ## VALUE_FIELD(int level, KEY_TYPE key, VALUE_TYPE value);

// #define DEF_SKIP_LIST_FIND(KEY_TYPE, KEY_FIELD, VALUE_TYPE, VALUE_FIELD) \
// skip_node_t *skip_list_find_ ## KEY_FIELD ## _ ## VALUE_FIELD(skip_list_t *l, KEY_TYPE key){ \
//     skip_node_t *cur = l->header; \
//     for (int i = l->level-1; i >= 0; i--) { \
//         while(cur->level[i] && cur->level[i]->key < key){ \ 
//             cur = cur->level[i]; \
//         } \
//         skip_node_t *next = cur->level[i]; \
//         if(next != NULL && next->key == key){ \
//             return next; \
//         } \
//     } \
//     return NULL; \
// }




DEF_SKIP_LIST(uint32_t, u32, uint32_t, u32)
DEF_SKIP_LIST(char *, s, void *, p)


int main(){
    element_t key;
    element_t value;

    skip_list_t *u32_skiplist = skip_list_create_u32_u32();
    for(int i=0; i<20; i++){
        key.u32 = random() % 100;
        value.u32 = key.u32 * 2;

        u32_skiplist->insert_func(u32_skiplist, key, value);
    }

    u32_skiplist->print_func(u32_skiplist);


    static char * const words[] = {
        "firefox",
        "chrome",
        "opera",
        "bash",
        "fish",
        "zsh",
        "ksh",
        "csh",
        NULL
    };

    skip_list_t *str_skiplist = skip_list_create_s_p();
    
    for(int i=0; words[i]!=NULL; i++){
        key.s = words[i];
        value.p = NULL;
        str_skiplist->insert_func(str_skiplist, key, value);
    }
    str_skiplist->print_func(str_skiplist);

    return 0;
}

