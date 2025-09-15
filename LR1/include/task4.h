#ifndef TASK4_H
#define TASK4_H


#include "status.h"


typedef enum { M_SERIES=0, M_EQUATION=1, M_LIMIT=2 } method_t;


status_t calc_e(double eps, method_t m, double *out);
status_t calc_pi(double eps, method_t m, double *out);
status_t calc_ln2(double eps, method_t m, double *out);
status_t calc_sqrt2(double eps, method_t m, double *out);
status_t calc_gamma(double eps, method_t m, double *out);


#endif /* TASK4_H */

