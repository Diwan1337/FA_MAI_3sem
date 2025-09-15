#ifndef TASK2_H
#define TASK2_H

#include <stddef.h>
#include <stdint.h>
#include "status.h"

status_t nth_prime(uint64_t n, uint64_t *out);
status_t primes_by_queries(const uint64_t *ns, size_t T, uint64_t *outs);

#endif /* TASK2_H */
