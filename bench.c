/* v1 vs v2 整体性能对比
 * uint32_t key/value，1000 万元素，相同数据集
 */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

// === v1 ===
#define NDEBUG
#include "skiplist/skiplist.h"

// cleanup v1 foreach macros before v2 (signatures differ)
#undef skip_list_foreach
#undef skip_list_foreach_safe
#undef skip_list_foreach_reverse
#undef skip_list_foreach_reverse_safe

// === v2 ===
#include "skiplist2/skiplist2.h"
DECLARE_SKIP_LIST(uint32_t, u32, uint32_t, u32)
static int compare_u32_u32(uint32_t a, uint32_t b) { return a<b ? -1 : (a==b ? 0 : 1); }
DEF_SKIP_LIST(uint32_t, u32, uint32_t, u32)

#define N 10000000

static double now() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1e-9;
}

int main() {
    uint32_t *keys = malloc(sizeof(uint32_t) * N);
    uint32_t *probes = malloc(sizeof(uint32_t) * N);
    if (!keys || !probes) { fprintf(stderr, "malloc failed\n"); return 1; }

    srand(42);
    for (int i = 0; i < N; i++) {
        keys[i] = (uint32_t)rand();
        probes[i] = (uint32_t)rand();
    }

    double t0, t_v1, t_v2;
    unsigned long v1_found, v2_found;

    // ===== v1: insert + find + remove =====
    t0 = now();
    skip_list_t *v1 = SKIP_LIST_CREATE(uint32_t, uint32_t);
    for (int i = 0; i < N; i++) SKIP_LIST_INSERT(v1, keys[i], 0U);
    v1_found = 0;
    for (int i = 0; i < N; i++) if (SKIP_LIST_FIND(v1, probes[i])) v1_found++;
    for (int i = 0; i < N; i++) SKIP_LIST_REMOVE(v1, keys[i]);
    SKIP_LIST_DESTROY(v1);
    t_v1 = now() - t0;

    // ===== v2: insert + find + remove =====
    t0 = now();
    skip_list_u32_u32_t *v2 = skip_list_create_u32_u32();
    for (int i = 0; i < N; i++) v2->insert(v2, keys[i], 0U);
    v2_found = 0;
    for (int i = 0; i < N; i++) if (v2->find(v2, probes[i])) v2_found++;
    for (int i = 0; i < N; i++) v2->remove(v2, keys[i]);
    skip_list_destroy_u32_u32(v2);
    t_v2 = now() - t0;

    printf("========== v1 vs v2 (10M elements, -O3) ==========\n");
    printf("v1: %.1fs  v2: %.1fs  (v2 %+.0f%%)\n",
           t_v1, t_v2, (t_v2 - t_v1) / t_v1 * 100);
    printf("(v1 found %lu, v2 found %lu)\n", v1_found, v2_found);

    free(keys);
    free(probes);
    return 0;
}
