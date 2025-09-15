#include <ctype.h>
#include <string.h>
#include <limits.h>
#include "base_utils.h"

/* '0'..'9','A'..'Z','a'..'z' → 0..35; иначе -1 */
int char_to_digit(int ch) {
    if (ch >= '0' && ch <= '9') return ch - '0';
    if (ch >= 'A' && ch <= 'Z') return ch - 'A' + 10;
    if (ch >= 'a' && ch <= 'z') return ch - 'a' + 10;
    return -1;
}

int digit_to_char(int d) {
    if (d >= 0 && d <= 9)  return '0' + d;
    if (d >= 10 && d < 36) return 'A' + (d - 10);
    return '?';
}

size_t trim_leading_zeros(const char **ps, size_t len) {
    const char *s = *ps;
    size_t i = 0;
    while (i < len && s[i] == '0') ++i;
    *ps = s + i;
    return len - i;
}

status_t minimal_valid_base(const char *s, int *base_out) {
    if (!s || !base_out) return ST_ERR_ARGS;

    while (isspace((unsigned char)*s)) ++s;
    if (*s == '+' || *s == '-') ++s;

    if (*s == '\0') return ST_ERR_PARSE;

    int maxd = -1, any = 0;
    for (const char *p = s; *p; ++p) {
        if (isspace((unsigned char)*p)) break;
        int d = char_to_digit((unsigned char)*p);
        if (d < 0) return ST_ERR_PARSE;
        if (d > maxd) maxd = d;
        any = 1;
    }
    if (!any) return ST_ERR_PARSE;

    int base = (maxd + 1 < 2) ? 2 : (maxd + 1);
    if (base > 36) return ST_ERR_RANGE;
    *base_out = base;
    return ST_OK;
}

int abs_compare_in_base(const char *a, const char *b, int base) {
    while (isspace((unsigned char)*a)) ++a;
    while (isspace((unsigned char)*b)) ++b;
    if (*a == '+' || *a == '-') ++a;
    if (*b == '+' || *b == '-') ++b;

    const char *pa = a, *pb = b;
    size_t la = 0, lb = 0;
    while (pa[la] && !isspace((unsigned char)pa[la])) ++la;
    while (pb[lb] && !isspace((unsigned char)pb[lb])) ++lb;

    const char *a0 = a; size_t la0 = la;
    const char *b0 = b; size_t lb0 = lb;
    la0 = trim_leading_zeros(&a0, la0);
    lb0 = trim_leading_zeros(&b0, lb0);

    if (la0 < lb0) return -1;
    if (la0 > lb0) return  1;

    for (size_t i = 0; i < la0; ++i) {
        int da = char_to_digit((unsigned char)a0[i]);
        int db = char_to_digit((unsigned char)b0[i]);
        if (da < 0 || da >= base || db < 0 || db >= base) {
            if (da == db) continue;
            return (da < db) ? -1 : 1;
        }
        if (da != db) return (da < db) ? -1 : 1;
    }
    return 0;
}

status_t to_int64_base(const char *s, int base, long long *out) {
    if (!s || !out) return ST_ERR_ARGS;
    if (base < 2 || base > 36) return ST_ERR_RANGE;

    while (isspace((unsigned char)*s)) ++s;

    int neg = 0;
    if (*s == '+' || *s == '-') { neg = (*s == '-'); ++s; }

    int any = 0;
    long long val = 0;

    for (; *s && !isspace((unsigned char)*s); ++s) {
        int d = char_to_digit((unsigned char)*s);
        if (d < 0 || d >= base) return ST_ERR_PARSE;
        any = 1;

        if (!neg) {
            if (val > (LLONG_MAX - d) / base) return ST_ERR_OVERFLOW;
            val = val * base + d;
        } else {
            if (val < (LLONG_MIN + d) / base) return ST_ERR_OVERFLOW;
            val = val * base - d;
        }
    }
    if (!any) return ST_ERR_PARSE;

    *out = val;
    return ST_OK;
}
