#ifndef BASE_UTILS_H
#define BASE_UTILS_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "status.h"

/* Перевод символа в цифру (0..35), -1 если не цифра (регистр не важен) */
int char_to_digit(int ch);
/* Перевод цифры (0..35) в символ ('0'..'9','A'..'Z') */
int digit_to_char(int d);
/* Обрезать ведущие нули "на месте": сдвигает указатель и возвращает новую длину */
size_t trim_leading_zeros(const char **ps, size_t len);
/* Найти минимально допустимую базу (2..36) по максимальной цифре в строке (со знаком) */
status_t minimal_valid_base(const char *s, int *base_out);
/* Сравнить |a| и |b| в заданной базе (без перевода в десятичную):
   -1 если |a|<|b|, 0 если равны, 1 если |a|>|b| */
int abs_compare_in_base(const char *a, const char *b, int base);
/* Перевод (знаковая строка в системе base) в long long; OVERFLOW если не влезает */
status_t to_int64_base(const char *s, int base, long long *out);

#endif /* BASE_UTILS_H */
