#ifndef FLOAT_UTILS_H
#define FLOAT_UTILS_H

#include <stdbool.h>

bool feq(double a, double b, double eps);
bool flt(double a, double b, double eps); // a < b с допуском
bool fle(double a, double b, double eps); // a <= b с допуском

#endif /* FLOAT_UTILS_H */
