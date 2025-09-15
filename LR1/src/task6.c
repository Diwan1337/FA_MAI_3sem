#include <math.h>
#include <float.h>
#include "task6.h"


static inline double sinc(double x){ return (x==0.0)?1.0:sin(x)/x; }


double f1(double x){ return exp(-x*x); }


double f2(double x){ return sinc(x); }


static double trap_step(func_t f, double a, double b, int n){
	double h=(b-a)/n; double s=0.5*(f(a)+f(b)); for (int i=1;i<n;++i) s+=f(a+i*h); return s*h;
}


double integrate_trapezoid(func_t f, double a, double b, double eps){
	int n=4; double prev=trap_step(f,a,b,n); for(;;){ n*=2; double cur=trap_step(f,a,b,n); if (fabs(cur-prev)<=eps || n> (1<<26)) return cur; prev=cur; }
}


