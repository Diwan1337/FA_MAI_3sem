#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include "common.h"
#include <stdint.h>

static const char* skip_spaces(const char* s){
    while (s && *s && isspace((unsigned char)*s)) ++s;
    return s;
}

/* Считает «префикс знаков»: +++--+-42 → знак=минус (3 минуса → нечётно) и возвращает позицию после знаков */
static const char* eat_sign_run(const char* s, int* sign_out){
    int minus = 0;
    int any = 0;
    while (*s == '+' || *s == '-') { if (*s=='-') ++minus; ++s; any=1; }
    if (sign_out) *sign_out = (minus % 2 == 0) ? +1 : -1;
    return s;
}

status_t parse_int64(const char *s, int64_t *out) {
    if (!s || !out) return ST_ERR_ARGS;

    const char *p = s;

    // 1) пропустить ведущие пробелы
    while (isspace((unsigned char)*p)) ++p;

    // 2) собрать последовательность знаков
    int neg = 0; // 0 -> +, 1 -> -
    int saw_sign = 0;
    while (*p == '+' || *p == '-') {
        saw_sign = 1;
        if (*p == '-') neg ^= 1; // меняем чётность минусов
        ++p;
    }

    // 3) обязателен хотя бы один десятичный символ
    if (!isdigit((unsigned char)*p)) return ST_ERR_PARSE;

    // 4) парс цифр с проверкой переполнения
    int64_t val = 0;
    for (; *p; ++p) {
        if (isspace((unsigned char)*p)) {
            // допускаем только хвостовые пробелы
            while (isspace((unsigned char)*p)) ++p;
            if (*p != '\0') return ST_ERR_PARSE;
            break;
        }
        if (!isdigit((unsigned char)*p)) return ST_ERR_PARSE;
        int d = *p - '0';
        if (!neg) {
            if (val > (INT64_MAX - d) / 10) return ST_ERR_OVERFLOW;
            val = val * 10 + d;
        } else {
            // для отрицательных бережём нижнюю границу
            if (val < (INT64_MIN + d) / 10) return ST_ERR_OVERFLOW;
            val = val * 10 - d; // накапливаем сразу со знаком
        }
    }

    *out = val;
    return ST_OK;
}

status_t safe_sum_1_to_n(int64_t n, int64_t *out) {
    if (!out) return ST_ERR_ARGS;
    if (n < 0) return ST_ERR_RANGE;
    // Вычисляем n*(n+1)/2 без __int128 и без переполнения
    int64_t a, b;
    if ((n & 1) == 0) { // n чётное: (n/2)*(n+1)
        a = n / 2;
        b = n + 1;
    } else { // n нечётное: ((n+1)/2)*n
        a = (n + 1) / 2;
        b = n;
    }
    if (a != 0 && b > INT64_MAX / a) return ST_ERR_OVERFLOW;
    *out = a * b;
    return ST_OK;
}

status_t safe_factorial_u64(uint64_t n, uint64_t *out) {
    if (!out) return ST_ERR_ARGS;
    uint64_t r = 1;
    for (uint64_t i = 2; i <= n; ++i) {
        if (r > UINT64_MAX / i) return ST_ERR_OVERFLOW;
        r *= i;
    }
    *out = r;
    return ST_OK;
}

bool is_prime_u64(uint64_t x) {
    if (x <= 1) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t d = 3; d * d <= x; d += 2) {
        if (x % d == 0) return false;
    }
    return true;
}
