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

status_t task3_t_is_right_triangle(double eps, double a, double b, double c) {
    if (a <= eps || b <= eps || c <= eps) {
        fprintf(stdout, "Не треугольник (сторона <= 0)\n");
        return ST_ERR_RANGE;
    }
    double sides[3] = {a, b, c};
    for (int i = 0; i < 2; i++) {
        for (int j = i + 1; j < 3; j++) {
            if (sides[i] < sides[j]) {
                double temp = sides[i];
                sides[i] = sides[j];
                sides[j] = temp;
            }
        }
    }
    double c_sq = sides[0] * sides[0];
    double sum_sq = sides[1] * sides[1] + sides[2] * sides[2];
    if (feq(c_sq, sum_sq, eps)) {
        fprintf(stdout, "Прямоугольный треугольник\n");
    } else {
        fprintf(stdout, "Не прямоугольный треугольник\n");
    }
    return ST_OK;
}