#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "task2.h"

int main(void) {
    uint64_t T = 0;
    if (scanf("%" SCNu64, &T) != 1 || T == 0) return ST_ERR_ARGS;
    uint64_t *qs = malloc(sizeof(uint64_t) * (size_t)T);
    for (uint64_t i = 0; i < T; ++i) if (scanf("%" SCNu64, &qs[i]) != 1) return ST_ERR_PARSE;
    uint64_t *ans = malloc(sizeof(uint64_t) * (size_t)T);
    if (primes_by_queries(qs, (size_t)T, ans) != ST_OK) return ST_ERR_IO;
    for (uint64_t i = 0; i < T; ++i) printf("%" PRIu64 "\n", ans[i]);
    free(qs); free(ans);
    return ST_OK;
}
