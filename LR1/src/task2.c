#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <limits.h>
#include "task2.h"

static uint64_t upper_bound_nth_prime(uint64_t n) {
    if (n < 6) {
        const uint64_t small[] = {0,2,3,5,7,11};
        return small[n];
    }
    long double nn = (long double)n;
    long double bound = nn * (logl(nn) + logl(logl(nn)));
    bound *= 1.2L;
    if (bound < nn) bound = nn;
    if (bound > (long double)UINT64_MAX) return UINT64_MAX;
    return (uint64_t)ceill(bound);
}

static status_t sieve_up_to(uint64_t N, unsigned char **out, uint64_t *len) {
    if (!out || !len) return ST_ERR_ARGS;
    if (N < 2) N = 2;
    size_t sz = (size_t)(N + 1);
    unsigned char *mark = malloc(sz);
    if (!mark) return ST_ERR_IO;
    memset(mark, 0, sz);
    mark[0] = mark[1] = 1;
    uint64_t limit = (uint64_t)sqrt((long double)N);
    for (uint64_t p = 2; p <= limit; ++p)
        if (mark[p] == 0)
            for (uint64_t m = p * p; m <= N; m += p) mark[m] = 1;
    *out = mark;
    *len = N + 1;
    return ST_OK;
}

status_t nth_prime(uint64_t n, uint64_t *out) {
    if (!out) return ST_ERR_ARGS;
    if (n == 0) return ST_ERR_RANGE;
    uint64_t N = upper_bound_nth_prime(n);
    unsigned char *mark = NULL; uint64_t len = 0;
    if (sieve_up_to(N, &mark, &len) != ST_OK) return ST_ERR_IO;
    uint64_t cnt = 0, ans = 0;
    for (uint64_t i = 2; i < len; ++i) if (mark[i] == 0 && ++cnt == n) { ans = i; break; }
    free(mark);
    if (!ans) return ST_ERR_RANGE;
    *out = ans;
    return ST_OK;
}

status_t primes_by_queries(const uint64_t *ns, size_t T, uint64_t *outs) {
    if (!ns || !outs) return ST_ERR_ARGS;
    uint64_t maxn = 0;
    for (size_t i = 0; i < T; ++i) if (ns[i] > maxn) maxn = ns[i];
    uint64_t N = upper_bound_nth_prime(maxn);
    unsigned char *mark = NULL; uint64_t len = 0;
    if (sieve_up_to(N, &mark, &len) != ST_OK) return ST_ERR_IO;
    uint64_t *pr = malloc(sizeof(uint64_t) * (size_t)maxn);
    if (!pr) { free(mark); return ST_ERR_IO; }
    uint64_t cnt = 0;
    for (uint64_t i = 2; i < len && cnt < maxn; ++i) if (mark[i] == 0) pr[cnt++] = i;
    free(mark);
    for (size_t i = 0; i < T; ++i) outs[i] = pr[ns[i]-1];
    free(pr);
    return ST_OK;
}
