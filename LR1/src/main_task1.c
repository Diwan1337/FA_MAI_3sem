#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "common.h"
#include "task1.h"

static void print_usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s <x> <-h|-p|-s|-e|-a|-f>\n", prog);
}

int main(int argc, char **argv) {
    if (argc != 3) { print_usage(argv[0]); return ST_ERR_ARGS; }
    int64_t x = 0;
    if (parse_int64(argv[1], &x) != ST_OK) return ST_ERR_PARSE;
    const char *flag = argv[2];
    if (strcmp(flag, "-h") == 0) return task1_h_multiples_upto_100(x);
    else if (strcmp(flag, "-p") == 0) return task1_p_is_prime(x);
    else if (strcmp(flag, "-s") == 0) return task1_s_split_hex_digits(x);
    else if (strcmp(flag, "-e") == 0) return task1_e_power_table(x);
    else if (strcmp(flag, "-a") == 0) return task1_a_sum_1_to_x(x);
    else if (strcmp(flag, "-f") == 0) return task1_f_factorial(x);
    else { print_usage(argv[0]); return ST_ERR_ARGS; }
}
