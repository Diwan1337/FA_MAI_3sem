#include <stdio.h>
#include <math.h>
#include <inttypes.h>
#include "task3.h"
#include "float_utils.h"

status_t task3_q_solve_quad(double eps, double a, double b, double c) {
    if (feq(a, 0.0, eps)) { 
        fprintf(stdout, "Не квадратное уравнение\n"); 
        return ST_ERR_RANGE; 
    }
    double D = b*b - 4*a*c;
    if (D < -eps) { 
        fprintf(stdout, "Нет действительных корней\n"); 
        return ST_OK; 
    }
    else if (feq(D, 0.0, eps)) { 
        fprintf(stdout, "x=%g\n", -b/(2*a)); 
    }
    else {
        double sqrtD = sqrt(fabs(D));
        fprintf(stdout, "x1=%g x2=%g\n", (-b-sqrtD)/(2*a), (-b+sqrtD)/(2*a));
    }
    return ST_OK;
}

status_t task3_m_is_multiple(int64_t a, int64_t b) {
    if (b == 0) {
        fprintf(stdout, "Деление на ноль\n");
        return ST_ERR_RANGE;
    }
    if (a % b == 0) {
        fprintf(stdout, "%" PRId64 " кратно %" PRId64 "\n", a, b);
    } else {
        fprintf(stdout, "%" PRId64 " не кратно %" PRId64 "\n", a, b);
    }
    return ST_OK;
}

status_t task3_t_is_right_triangle(double eps, double x, double y, double z) {
    // eps должен быть строго > 0
    if (!(eps > 0)) return ST_ERR_RANGE;

    // стороны тоже строго положительные
    if (!(x > 0 && y > 0 && z > 0)) return ST_ERR_RANGE;

    // далее как было...
    double a = x, b = y, c = z;
    if (a > b) { double t=a; a=b; b=t; }
    if (b > c) { double t=b; b=c; c=t; }
    if (a > b) { double t=a; a=b; b=t; }

    // проверка существования треугольника: c < a + b (с допуском)
    if (!flt(c, a + b, eps)) {
        printf("не треугольник\n");
        return ST_OK;
    }

    double lhs = a*a + b*b;
    double rhs = c*c;
    if (feq(lhs, rhs, eps)) printf("прямоугольный\n");
    else printf("не прямоугольный\n");
    return ST_OK;
}