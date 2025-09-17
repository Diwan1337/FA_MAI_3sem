#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "common.h"
#include "task1.h"
#include <ctype.h>

static int str_ieq(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        ++a; ++b;
    }
    return *a == '\0' && *b == '\0';
}

static int flag_is(const char* s, const char* one_letter) {
    if (!s || !*s) return 0;
    if (s[0] != '-' && s[0] != '/') return 0;
    ++s;
    if (*s == '-') ++s;
    return s[0] && !s[1] && tolower((unsigned char)s[0]) == tolower((unsigned char)one_letter[0]);
}

static void print_usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s <x> <-h|-p|-s|-e|-a|-f>\n", prog);
}

int main(int argc, char **argv) {
    if (argc != 3) { print_usage(argv[0]); return ST_ERR_ARGS; }
    int64_t x = 0;
    if (parse_int64(argv[1], &x) != ST_OK) return ST_ERR_PARSE;
    const char *flag = argv[2];
    if (flag_is(flag, "h")) return task1_h_multiples_upto_100(x);
    else if (flag_is(flag, "p")) return task1_p_is_prime(x);
    else if (flag_is(flag, "s")) return task1_s_split_hex_digits(x);
    else if (flag_is(flag, "e")) return task1_e_power_table(x);
    else if (flag_is(flag, "a")) return task1_a_sum_1_to_x(x);
    else if (flag_is(flag, "f")) return task1_f_factorial(x);
    else { print_usage(argv[0]); return ST_ERR_ARGS; }
}
