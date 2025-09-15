#include <math.h>
#include "float_utils.h"

bool feq(double a, double b, double eps) {
    return fabs(a - b) <= eps;
}

bool flt(double a, double b, double eps) {
    return (a < b) && !feq(a, b, eps);
}

bool fle(double a, double b, double eps) {
    return (a < b) || feq(a, b, eps);
}
