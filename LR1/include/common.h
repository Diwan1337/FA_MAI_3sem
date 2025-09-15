#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include "status.h"

status_t parse_int64(const char *s, int64_t *out);
status_t safe_sum_1_to_n(int64_t n, int64_t *out);
status_t safe_factorial_u64(uint64_t n, uint64_t *out);
bool is_prime_u64(uint64_t x);

#endif /* COMMON_H */
