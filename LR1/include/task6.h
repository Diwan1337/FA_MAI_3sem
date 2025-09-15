#ifndef TASK6_H
#define TASK6_H


#include "status.h"


typedef double (*func_t)(double x);


double integrate_trapezoid(func_t f, double a, double b, double eps);


/* Заглушка: конкретные интегралы №6 из методички нужно подставить сюда.
 * Сделаны примеры f1..f3, при необходимости добавим остальные, когда закрепим формулы. */
double f1(double x); // пример: exp(-x^2)
double f2(double x); // пример: sin(x)/x (sinc), sinc(0)=1


typedef enum { I_F1=1, I_F2=2 } intg_id;


#endif /* TASK6_H */
