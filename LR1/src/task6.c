#include <math.h>
#include <float.h>
#include "task6.h"

static inline double sinc(double x){
    const double tiny = 1e-8;
    if (fabs(x) < tiny) {
        /* Разложение sin(x)/x = 1 - x^2/6 + O(x^4) для малых x */
        return 1.0 - (x*x)/6.0;
    }
    return sin(x)/x;
}

double f1(double x){ return exp(-x*x); }
double f2(double x){ return sinc(x); }

/* Базовая формула Симпсона на [a,b] */
static inline double simpson(func_t f, double a, double b){
    const double c = 0.5*(a+b);
    return (f(a) + 4.0*f(c) + f(b)) * (b-a) / 6.0;
}

/* Адаптивный Симпсон с контролем точности.
   S = Simpson(a,b); рекурсивно делим отрезок, пока |S_L+S_R - S| <= 15*eps.
   Возвращаем S_L + S_R + поправку Ричардсона. */
static double adaptive_simpson(func_t f, double a, double b, double eps,
                               double whole, int depth)
{
    const double c  = 0.5*(a+b);
    const double S1 = simpson(f, a, c);
    const double S2 = simpson(f, c, b);
    const double diff = S1 + S2 - whole;

    /* критерий остановки: абсолютный ИЛИ относительный (относительно whole)
       также прекращаем при исчерпании глубины */
    if (depth <= 0) {
        return S1 + S2 + diff/15.0;
    }
    double abs_ok = fabs(diff) <= 15.0 * eps;
    double rel_ok = fabs(diff) <= 15.0 * eps * fmax(1.0, fabs(whole));
    if (abs_ok || rel_ok) {
        return S1 + S2 + diff/15.0;
    }

    /* делим эпсилон пополам на подотрезки */
    const double left  = adaptive_simpson(f, a, c, 0.5*eps, S1, depth-1);
    const double right = adaptive_simpson(f, c, b, 0.5*eps, S2, depth-1);
    return left + right;
}

double integrate_trapezoid(func_t f, double a, double b, double eps){
    if (a == b) return 0.0;
    int neg = 0;
    if (a > b) { double t=a; a=b; b=t; neg=1; }

    /* стартовая оценка Симпсона на всем интервале */
    const double whole = simpson(f, a, b);
    /* глубина ~ 30 хватит с головой (2^30 делений невозможны; остановимся раньше по точности) */
    double res = adaptive_simpson(f, a, b, eps, whole, 32);
    return neg ? -res : res;
}
