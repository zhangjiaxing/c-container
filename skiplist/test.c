#define NDEBUG

#include "skiplist.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>


#define K 1000
#define M (1000*1000)

void test_int32(){
    fprintf(stderr, "\n=============== [ %s ] ================\n", __func__);

    skip_list_t *i32_skiplist = SKIP_LIST_CREATE(int32_t, int32_t);

    for(int i=0; i<20; i++){
        int32_t n = rand() % 100;
        SKIP_LIST_INSERT_MULTI(i32_skiplist, n, -n);
    }
    SKIP_LIST_INSERT_MULTI(i32_skiplist, 11, -11);
    SKIP_LIST_INSERT_MULTI(i32_skiplist, 11, -11);
    SKIP_LIST_INSERT(i32_skiplist, 1234, -1234);
    SKIP_LIST_INSERT(i32_skiplist, 1234, -1234);


    skip_list_addr_print(i32_skiplist);

    skip_node_t *node;
    node = SKIP_LIST_FIND(i32_skiplist, 56);
    if(node != NULL){
        printf("found key: %d, value is: %d\n", node->key.i32, node->value.i32);
        int rank = SKIP_LIST_GET_NODE_RANK(i32_skiplist, node);
        printf("node rank = %d\n", rank);
        SKIP_LIST_REMOVE_NODE(i32_skiplist, node);
        skip_list_print(i32_skiplist);
    }else{
        printf("not found\n");
    }

    SKIP_LIST_DESTROY(i32_skiplist);
}

void test_uint32_bench(){
    fprintf(stderr, "\n=============== [ %s ] ================\n", __func__);

    uint32_t *data = malloc(sizeof(uint32_t) * 10 * M);
    for(int i=0; i<10*M; i++){
        data[i] = rand();
    }

    clock_t t1 = clock();
    skip_list_t *u32_skiplist = SKIP_LIST_CREATE(uint32_t, uint32_t);
    for(int i=0; i<10*M; i++){
        SKIP_LIST_INSERT(u32_skiplist, data[i], 0U);
    }
    clock_t t2 = clock();

    printf("time : %f s\n", ((double)(t2-t1))/CLOCKS_PER_SEC);
    SKIP_LIST_DESTROY(u32_skiplist);
    free(data);
}

void test_type_err(){
    fprintf(stderr, "\n=============== [ %s ] ================\n", __func__);

    skip_list_t *double_skiplist = SKIP_LIST_CREATE(double, int32_t);
    SKIP_LIST_DESTROY(double_skiplist);
}

void test_srt(){
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

    skip_list_t *str_skiplist = SKIP_LIST_CREATE(char *, void *);

    for(int i=0; words[i]!=NULL; i++){
        SKIP_LIST_INSERT(str_skiplist, words[i], NULL);
    }
    
    int rank = SKIP_LIST_GET_RANK(str_skiplist, ((char *)"opera"));
    printf("opera node rank = %d\n", rank);
    SKIP_LIST_REMOVE(str_skiplist, "opera");
    skip_list_print(str_skiplist);

    fprintf(stderr, "\n=========== test reverse\n");
    skip_node_t *node;
    skip_list_foreach_reverse(node, str_skiplist) {
        printf("%s-", node->key.s);
    }
    printf("\n\n");


    fprintf(stderr, "test skip_list_for_each_reverse_safe\n");
    skip_list_foreach_reverse_safe(node, str_skiplist){
        if(strcmp(node->key.s, "chrome") == 0){
            bool ret = SKIP_LIST_REMOVE_NODE(str_skiplist, node);
            printf("delete node %p (%s): \n", node, ret == true? "ok": "failed");
        }
    }
    
    printf("\n");
    skip_list_rank_print(str_skiplist);
    printf("\n");

    printf("SKIP_LIST_GET_RANK(\"firefox\") == %lu\n", SKIP_LIST_GET_RANK(str_skiplist, "firefox"));

    // node = SKIP_LIST_FIND(str_skiplist, "firefox");
    // int rank = SKIP_LIST_GET_NODE_RANK(str_skiplist, node);
    printf("SKIP_LIST_GET_NODE_RANK(SKIP_LIST_GET_NODE_BY_RANK(\"firefox\")) == %lu \n", SKIP_LIST_GET_NODE_RANK(str_skiplist, SKIP_LIST_FIND(str_skiplist, "firefox")));

    fprintf(stderr, "test skip_list_for_each_safe\n");
    skip_list_foreach_safe(node, str_skiplist){
        printf("%s-", node->key.s);
    }
    printf("\n");

    SKIP_LIST_DESTROY(str_skiplist);
}

int main(){

    test_int32();

    test_uint32_bench();
    
    test_srt();

    test_type_err();

    return 0;
}

