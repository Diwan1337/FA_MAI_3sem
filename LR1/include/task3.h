#ifndef TASK3_H
#define TASK3_H

#include <stdint.h>
#include "status.h"

status_t task3_q_solve_quad(double eps, double a, double b, double c);
status_t task3_m_is_multiple(int64_t a, int64_t b);
status_t task3_t_is_right_triangle(double eps, double a, double b, double c);

#endif /* TASK3_H */
