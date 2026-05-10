#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>

// 1. 包含头文件，声明类型和函数原型
#include "skiplist2.h"
DECLARE_SKIP_LIST(char *, s, int32_t, int32)
DECLARE_SKIP_LIST(int32_t, i32, int32_t, i32)
DECLARE_SKIP_LIST(uint32_t, u32, uint32_t, u32)
DECLARE_SKIP_LIST(char *, s, void *, v)
DECLARE_SKIP_LIST(double, d, int32_t, i32)

// 2. 包含实现文件（包含宏定义 + random_level）
#include "skiplist2.c"

// 3. 定义比较函数（命名约定：compare_##KNAME##_##VNAME）
static int compare_s_int32(char *a, char *b) { return strcmp(a, b); }
static int compare_i32_i32(int32_t a, int32_t b) { return a<b ? -1 : (a==b ? 0 : 1); }
static int compare_u32_u32(uint32_t a, uint32_t b) { return a<b ? -1 : (a==b ? 0 : 1); }
static int compare_s_v(char *a, char *b) { return strcmp(a, b); }
static int compare_d_i32(double a, double b) { return a<b ? -1 : (a==b ? 0 : 1); }

// 4. 用一条宏生成所有函数定义
//    print 需要 C11 _Generic，通过编译时 -DSKIPLIST_USE_PRINT=1 开启
DEF_SKIP_LIST(char *, s, int32_t, int32)
DEF_SKIP_LIST(int32_t, i32, int32_t, i32)
DEF_SKIP_LIST(uint32_t, u32, uint32_t, u32)
DEF_SKIP_LIST(char *, s, void *, v)
DEF_SKIP_LIST(double, d, int32_t, i32)
#if SKIPLIST_USE_PRINT
DEF_SKIP_LIST_PRINT(char *, s, int32_t, int32)
DEF_SKIP_LIST_PRINT(int32_t, i32, int32_t, i32)
DEF_SKIP_LIST_PRINT(uint32_t, u32, uint32_t, u32)
DEF_SKIP_LIST_PRINT(char *, s, void *, v)
DEF_SKIP_LIST_PRINT(double, d, int32_t, i32)
#endif


/* ========== 保留的 skiplist2 原有测试 ========== */

void test_basic() {
    fprintf(stderr, "========== %s ==========\n", __func__);
    skip_list_s_int32_t *list = skip_list_create_s_int32();
#if SKIPLIST_USE_PRINT
    list->print = skip_list_print_s_int32;
#endif

    list->insert(list, "apple", 100);
    list->insert(list, "banana", 200);
    list->insert(list, "cherry", 300);

    skip_node_s_int32_t *dup = list->insert(list, "apple", 999);
    printf("insert dup apple: %s (expect NULL)\n", dup ? "node" : "NULL");

    skip_node_s_int32_t *node = list->find(list, "banana");
    printf("find banana: key=%s value=%d (expect 200)\n", node ? node->key : "NULL", node ? node->value : -1);

    node = list->find(list, "notfound");
    printf("find notfound: %s (expect NULL)\n", node ? "found" : "NULL");

    unsigned long rank = list->get_rank(list, "cherry");
    printf("rank of cherry: %lu (expect 3)\n", rank);

    rank = list->get_rank(list, "notfound");
    printf("rank of notfound: %lu (expect 0)\n", rank);

    node = list->get_node_by_rank(list, 1);
    printf("node at rank 1: %s (expect apple)\n", node ? node->key : "NULL");

    node = list->get_node_by_rank(list, 3);
    printf("node at rank 3: %s (expect cherry)\n", node ? node->key : "NULL");

    bool ret = list->remove(list, "banana");
    printf("remove banana: %d (expect 1)\n", ret);
#if SKIPLIST_USE_PRINT
    list->print(list);
#endif

    node = list->find(list, "apple");
    ret = list->remove_node(list, node);
    printf("remove_node apple: %d (expect 1)\n", ret);
#if SKIPLIST_USE_PRINT
    list->print(list);
#endif

    skip_list_destroy_s_int32(list);
}

void test_multi() {
    fprintf(stderr, "\n========== %s ==========\n", __func__);
    skip_list_s_int32_t *list = skip_list_create_s_int32();
#if SKIPLIST_USE_PRINT
    list->print = skip_list_print_s_int32;
#endif

    list->insert_multi(list, "x", 1);
    list->insert_multi(list, "x", 2);
    list->insert_multi(list, "x", 3);
    list->insert_multi(list, "y", 10);
#if SKIPLIST_USE_PRINT
    list->print(list);
#endif

    skip_list_destroy_s_int32(list);
}


/* ========== 从 skiplist v1 移植的测试 ========== */

#define K 1000
#define M (1000*1000)

void test_int32() {
    fprintf(stderr, "\n=============== [ %s ] ================\n", __func__);

    skip_list_i32_i32_t *list = skip_list_create_i32_i32();
#if SKIPLIST_USE_PRINT
    list->print = skip_list_print_i32_i32;
#endif

    for(int i=0; i<20; i++){
        int32_t n = rand() % 100;
        list->insert_multi(list, n, -n);
    }
    list->insert_multi(list, 11, -11);
    list->insert_multi(list, 11, -11);
    list->insert(list, 1234, -1234);
    skip_node_i32_i32_t *dup = list->insert(list, 1234, -1234);
    if (dup) printf("insert dup 1234: node (unexpected)\n");

#if SKIPLIST_USE_PRINT
    list->print(list);
#endif

    skip_node_i32_i32_t *node = list->find(list, 56);
    if(node != NULL){
        printf("found key: %d, value is: %d\n", node->key, node->value);
        unsigned long rank = list->get_node_rank(list, node);
        printf("node rank = %lu\n", rank);
        list->remove_node(list, node);
#if SKIPLIST_USE_PRINT
        list->print(list);
#endif
    }else{
        printf("not found\n");
    }

    skip_list_destroy_i32_i32(list);
}

void test_uint32_bench() {
    fprintf(stderr, "\n=============== [ %s ] ================\n", __func__);

    uint32_t *data = malloc(sizeof(uint32_t) * 10 * M);
    for(int i=0; i<10*M; i++){
        data[i] = rand();
    }

    clock_t t1 = clock();
    skip_list_u32_u32_t *list = skip_list_create_u32_u32();
    for(int i=0; i<10*M; i++){
        list->insert(list, data[i], 0U);
    }
    clock_t t2 = clock();

    printf("time : %f s\n", ((double)(t2-t1))/CLOCKS_PER_SEC);
    skip_list_destroy_u32_u32(list);
    free(data);
}

void test_srt() {
    fprintf(stderr, "\n=============== [ %s ] ================\n", __func__);

    static char * const words[] = {
        "firefox",
        "chrome",
        "opera",
        "bash",
        "fish",
        "zsh",
        "ksh",
        "csh",
        "dash",
        "vim",
        "emacs",
        "gedit",
        "LibreOffice",
        NULL
    };

    skip_list_s_v_t *list = skip_list_create_s_v();
#if SKIPLIST_USE_PRINT
    list->print = skip_list_print_s_v;
#endif

    for(int i=0; words[i]!=NULL; i++){
        list->insert(list, words[i], NULL);
    }

    unsigned long rank = list->get_rank(list, "opera");
    printf("opera node rank = %lu\n", rank);
    list->remove(list, "opera");

    rank = list->get_rank(list, "opera");
    printf("rank of deleted opera: %lu (expect 0)\n", rank);
#if SKIPLIST_USE_PRINT
    list->print(list);
#endif

    fprintf(stderr, "\n=========== test reverse\n");
    skip_node_s_v_t *cur;
    skip_list_foreach_reverse(cur, list) {
        printf("%s-", cur->key);
    }
    printf("\n\n");

    fprintf(stderr, "test skip_list_for_each_reverse_safe\n");
    {
        skip_node_s_v_t *cur, *tmp;
        skip_list_foreach_reverse_safe(cur, list, tmp) {
            if(strcmp(cur->key, "chrome") == 0){
                bool ret = list->remove_node(list, cur);
                printf("delete node %p (%s): \n", (void*)cur, ret == true? "ok": "failed");
            }
        }
    }

    printf("\n");
#if SKIPLIST_USE_PRINT
    list->print(list);
#endif
    printf("\n");

    printf("SKIP_LIST_GET_RANK(\"firefox\") == %lu\n", list->get_rank(list, "firefox"));
    printf("SKIP_LIST_GET_NODE_RANK(SKIP_LIST_GET_NODE_BY_RANK(\"firefox\")) == %lu \n",
        list->get_node_rank(list, list->find(list, "firefox")));

    fprintf(stderr, "test skip_list_for_each_safe\n");
    {
        skip_node_s_v_t *cur, *tmp;
        skip_list_foreach_safe(cur, list, tmp) {
            printf("%s-", cur->key);
        }
    }
    printf("\n");

    skip_list_destroy_s_v(list);
}

void test_double() {
    fprintf(stderr, "\n=============== [ %s ] ================\n", __func__);

    skip_list_d_i32_t *list = skip_list_create_d_i32();
#if SKIPLIST_USE_PRINT
    list->print = skip_list_print_d_i32;
#endif

    list->insert(list, 3.14, 100);
    list->insert(list, 2.71, 200);
    list->insert(list, 1.41, 300);

    skip_node_d_i32_t *node = list->find(list, 2.71);
    printf("find 2.71: %s (expect found)\n", node ? "found" : "NULL");
    if (node) printf("value=%d (expect 200)\n", node->value);

#if SKIPLIST_USE_PRINT
    list->print(list);
#endif

    skip_list_destroy_d_i32(list);
}


int main() {
    test_basic();
    test_multi();

    test_int32();
    test_uint32_bench();
    test_srt();
    test_double();

    fprintf(stderr, "\nAll tests passed.\n");
    return 0;
}
