#include "skiplist.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>



#define K 1000
#define M (1000*1000)


int main(){
    skip_list_t *i32_skiplist;
    i32_skiplist = SKIP_LIST_CREATE(int32_t, int32_t);

    element_t key;
    element_t value;
    int num_list[20];
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
    key.i32 = 56;
    node = SKIP_LIST_FIND_NODE(i32_skiplist, key.i32);
    if(node != NULL){
        fprintf(stderr, "found key: %d, value is: %d\n", node->key, node->value);
        int rank = SKIP_LIST_GET_NODE_RANK(i32_skiplist, node);
        fprintf(stderr, "node rank = %d\n", rank);
        SKIP_LIST_REMOVE_NODE(i32_skiplist, node);
        skip_list_print(i32_skiplist);
    }else{
        fprintf(stderr, "not found\n");
    }

    SKIP_LIST_DESTROY(i32_skiplist);

    fprintf(stderr, "=================================\n");

    int *data = malloc(sizeof(int) * 10 * M);
    for(int i=0; i<10*M; i++){
        data[i] = rand();
    }

    {
        clock_t t1 = clock();
        i32_skiplist = SKIP_LIST_CREATE(int32_t, int32_t);
        for(int i=0; i<10*M; i++){
            i32_skiplist->insert(i32_skiplist, (element_t)data[i], (element_t)0);
        }
        clock_t t2 = clock();

        printf("time : %f s\n", ((double)(t2-t1))/CLOCKS_PER_SEC);
    }


    // skip_list_t *double_skiplist;
    // double_skiplist = SKIP_LIST_CREATE(double, int32_t);

    fprintf(stderr, "=================================\n");
    
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

    skip_list_t *str_skiplist;
    str_skiplist = SKIP_LIST_CREATE(char *, void *);

    for(int i=0; words[i]!=NULL; i++){
        SKIP_LIST_INSERT(str_skiplist, words[i], NULL);
    }
    
    {
        int rank = SKIP_LIST_GET_RANK(str_skiplist, ((char *)"opera"));
        fprintf(stderr, "opera node rank = %d\n", rank);
    }
    SKIP_LIST_REMOVE(str_skiplist, "opera");
    skip_list_print(str_skiplist);
    // SKIP_LIST_DESTROY(str_skiplist);


    fprintf(stderr, "\n=========== test reverse\n");
    skip_list_foreach_reverse(node, str_skiplist) {
        fprintf(stderr, "%s-", node->key.s);
    }
    fprintf(stderr, "\n\n");


    fprintf(stderr, "test skip_list_for_each_reverse_safe\n");
    skip_list_foreach_reverse_safe(node, str_skiplist){
        if(strcmp(node->key.s, "chrome") == 0){
            bool ret = SKIP_LIST_REMOVE_NODE(str_skiplist, node);
            fprintf(stderr, "delete node %p (%s): \n", node, ret == true? "ok": "failed");
        }
    }
    
    fprintf(stderr, "\n");
    skip_list_rank_print(str_skiplist);
    fprintf(stderr, "\n");

    printf("SKIP_LIST_GET_RANK(\"firefox\") == %lu\n", SKIP_LIST_GET_RANK(str_skiplist, "firefox"));

    // node = SKIP_LIST_FIND_NODE(str_skiplist, "firefox");
    // int rank = SKIP_LIST_GET_NODE_RANK(str_skiplist, node);
    printf("SKIP_LIST_GET_NODE_RANK(SKIP_LIST_GET_NODE_BY_RANK(\"firefox\")) == %d \n", SKIP_LIST_GET_NODE_RANK(str_skiplist, SKIP_LIST_FIND_NODE(str_skiplist, "firefox")));


    fprintf(stderr, "test skip_list_for_each_safe\n");
    skip_list_foreach_safe(node, str_skiplist){
        fprintf(stderr, "%s-", node->key.s);
    }

    SKIP_LIST_DESTROY(str_skiplist);

    return 0;
}

