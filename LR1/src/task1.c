#include <stdio.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdlib.h>
#include <limits.h>
#include "task1.h"
#include "common.h"

status_t task1_h_multiples_upto_100(int64_t x) {
    if (x == 0) { fprintf(stdout, "Нет кратных (x=0)\n"); return ST_OK; }
    int printed = 0;
    for (int i = 1; i <= 100; ++i) {
        if (i % (x < 0 ? -x : x) == 0) {
            if (printed) fprintf(stdout, " ");
            fprintf(stdout, "%d", i);
            printed = 1;
        }
    }
    if (!printed) fprintf(stdout, "Кратных чисел в [1..100] нет\n");
    else fprintf(stdout, "\n");
    return ST_OK;
}

status_t task1_p_is_prime(int64_t x) {
    if (x < 0) x = -x;
    bool prime = is_prime_u64((uint64_t)x);
    if ((uint64_t)x <= 1) {
        fprintf(stdout, "%" PRId64 " не является ни простым, ни составным\n", x);
    } else if (prime) {
        fprintf(stdout, "%" PRId64 " — простое\n", x);
    } else {
        fprintf(stdout, "%" PRId64 " — составное\n", x);
    }
    return ST_OK;
}

status_t task1_s_split_hex_digits(int64_t x) {
    uint64_t ux = (x < 0) ? (uint64_t)(-(x + 1)) + 1u : (uint64_t)x;
    if (x < 0) {
        fprintf(stdout, "- ");
    }
    if (ux == 0) {
        fprintf(stdout, "0\n");
        return ST_OK;
    }
    int shift = 60;
    while (shift > 0 && ((ux >> shift) & 0xFULL) == 0) shift -= 4;
    int printed = 0;
    for (int s = shift; s >= 0; s -= 4) {
        uint64_t d = (ux >> s) & 0xFULL;
        char c = (d < 10) ? ('0' + (char)d) : ('A' + (char)(d - 10));
        if (printed) fprintf(stdout, " ");
        fprintf(stdout, "%c", c);
        printed = 1;
    }
    fprintf(stdout, "\n");
    return ST_OK;
}

status_t task1_e_power_table(int64_t x) {
    if (x < 1 || x > 10) return ST_ERR_RANGE;
    for (int base = 1; base <= 10; ++base) {
        fprintf(stdout, "%2d:", base);
        unsigned long long cur = 1ULL;
        for (int pow = 1; pow <= x; ++pow) {
            if (cur > ULLONG_MAX / (unsigned long long)base) {
                fprintf(stdout, " OVERFLOW");
                break;
            }
            cur *= (unsigned long long)base;
            fprintf(stdout, " %llu", cur);
        }
        fprintf(stdout, "\n");
    }
    return ST_OK;
}

status_t task1_a_sum_1_to_x(int64_t x) {
    int64_t res = 0;
    status_t st = safe_sum_1_to_n(x, &res);
    if (st != ST_OK) return st;
    fprintf(stdout, "%" PRId64 "\n", res);
    return ST_OK;
}

status_t task1_f_factorial(int64_t x) {
    if (x < 0) return ST_ERR_RANGE;
    uint64_t res = 1;
    status_t st = safe_factorial_u64((uint64_t)x, &res);
    if (st == ST_ERR_OVERFLOW) { fprintf(stdout, "OVERFLOW\n"); return st; }
    if (st != ST_OK) return st;
    fprintf(stdout, "%" PRIu64 "\n", res);
    return ST_OK;
}
