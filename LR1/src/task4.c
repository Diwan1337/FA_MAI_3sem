#include <math.h>
#include <stdio.h>
#include "task4.h"


static int stop_change(double prev, double cur, double eps){ return fabs(cur-prev) <= eps; }


status_t calc_e(double eps, method_t m, double *out){
	if (!(eps>0) || !out) return ST_ERR_ARGS;
	if (m==M_SERIES){
		double s=1.0, term=1.0; // 1/0!
		for (int n=1; n<1000000; ++n){
			term/=n; double prev=s; s+=term; if (fabs(s-prev)<=eps) { *out=s; return ST_OK; }
		}
		*out=s; return ST_OK;
	} else if (m==M_EQUATION){
		// Решаем ln y = 1 → f(y)=ln y -1; Ньютона
		double y=2.0; for (int i=0;i<1000;++i){ double fy=log(y)-1.0; double df=1.0/y; double ny=y - fy/df; if (fabs(ny-y)<=eps) { *out=ny; return ST_OK; } y=ny; }
		*out=y; return ST_OK;
	} else { // M_LIMIT
		double prev=0, cur=0; for (int n=1;n<10000000;++n){ cur=pow(1.0+1.0/n, n); if (n>1 && fabs(cur-prev)<=eps){ *out=cur; return ST_OK; } prev=cur; }
		*out=cur; return ST_OK;
	}
}
status_t calc_pi(double eps, method_t m, double *out){
	if (!(eps>0) || !out) return ST_ERR_ARGS;
	if (m==M_SERIES){
		// Грегори-Лейбниц: pi/4 = sum (-1)^n/(2n+1)
		double s=0.0, prev=0.0; for (long long n=0;n<200000000LL;++n){ double term = ((n&1)?-1.0:1.0)/(2.0*n+1.0); s+=term; if (n%1000==0){ double cur=4.0*s; if (n>0 && fabs(cur-prev)<=eps){ *out=cur; return ST_OK; } prev=cur; } }
		*out=4.0*s; return ST_OK;
	} else if (m==M_EQUATION){
		// Решим cos(x) = -1 на [3,4] (бисекция)
		double L=3.0, R=4.0, mid; double fL=cos(L)+1.0, fR=cos(R)+1.0; if (fL*fR>0) { /*fallback*/ }
		for (int i=0;i<2000;++i){ mid=0.5*(L+R); double fm=cos(mid)+1.0; if (fabs(fm)<=eps || 0.5*(R-L)<=eps) { *out=mid; return ST_OK; } if (fL*fm<=0){ R=mid; fR=fm; } else { L=mid; fL=fm; } }
		*out=mid; return ST_OK;
	} else { // M_LIMIT — произведение Уоллиса: pi/2 = Π (2n/(2n-1))*(2n/(2n+1))
		long double prod=1.0L; long double prev=0.0L; for (long long n=1;n<10000000LL;++n){ long double a=(2.0L*n)/(2.0L*n-1.0L); long double b=(2.0L*n)/(2.0L*n+1.0L); prod*=a*b; if ((n&65535)==0){ long double cur = 2.0L*prod; if (n>0 && fabsl(cur-prev)<=eps){ *out=(double)cur; return ST_OK; } prev=cur; } }
		*out=(double)(2.0L*prod); return ST_OK;
	}
}


status_t calc_ln2(double eps, method_t m, double *out){
	if (!(eps>0) || !out) return ST_ERR_ARGS;
	if (m==M_SERIES){
		// ln2 = 1 - 1/2 + 1/3 - 1/4 + ...
		double s=0.0, prev=0.0; for (long long n=1;n<200000000LL;++n){ double term = ((n&1)?1.0:-1.0)/n; s+=term; if (n%1000==0){ if (fabs(s-prev)<=eps){ *out=s; return ST_OK; } prev=s; } } *out=s; return ST_OK;
	} else if (m==M_EQUATION){
		// Решим exp(x)-2=0 (Ньютон)
		double x=0.7; for (int i=0;i<2000;++i){ double fx=exp(x)-2.0; double d=exp(x); double nx=x - fx/d; if (fabs(nx-x)<=eps){ *out=nx; return ST_OK; } x=nx; } *out=x; return ST_OK;
	} else { // n*(2^{1/n}-1)
		double prev=0, cur=0; for (int n=1;n<10000000;++n){ cur = n*(pow(2.0,1.0/n)-1.0); if (n>1 && fabs(cur-prev)<=eps){ *out=cur; return ST_OK; } prev=cur; } *out=cur; return ST_OK;
	}
}
status_t calc_sqrt2(double eps, method_t m, double *out){
	if (!(eps>0) || !out) return ST_ERR_ARGS;
	if (m==M_SERIES){
		// 1/sqrt(1-t) = sum C(2n,n) t^n / 4^n; подставим t=1/2 => sqrt(2)
		long double t=0.5L; long double s=1.0L, prev=0.0L; long double c=1.0L; // C(0,0)=1
		for (long long n=1;n<1000000LL;++n){ // c = C(2n,n)
			// рекуррентно: C(2n,n) = C(2n-2,n-1) * (4n-2)/(n)
			c = c * (4.0L*n - 2.0L) / (long double)n;
			long double term = c * powl(t, n) / powl(4.0L, n);
			s += term;
			if (fabsl(s-prev)<=eps){ *out=(double)s; return ST_OK; }
			prev=s;
		}
		*out=(double)s; return ST_OK;
	} else if (m==M_EQUATION){
		// Решаем x^2 - 2 = 0 (Ньютон)
		double x=1.4; for (int i=0;i<200;++i){ double fx=x*x-2.0; double d=2.0*x; double nx=x - fx/d; if (fabs(nx-x)<=eps){ *out=nx; return ST_OK; } x=nx; } *out=x; return ST_OK;
	} else { // M_LIMIT — произведение Виета: 2/π * π? Используем двоичную среднюю: AGM дает 1/AGM(1,1/√2) = π/2; но это про π. Возьмём итерацию Герона как предел
		// Герон: x_{к+1} = (x_k + 2/x_k)/2
		double x=1.0; double prev=0.0; for (int k=0;k<100000;++k){ double nx=0.5*(x + 2.0/x); if (fabs(nx-x)<=eps){ *out=nx; return ST_OK; } x=nx; prev=x; } *out=x; return ST_OK;
	}
}


// Для гаммы используем: (1) предел H_n - ln n; (2) интеграл γ = ∫_0^1 (1/(1-t) - 1/ln t) dt (численно);
// (3) Лимитная формула Стерлинга: γ = lim_{n→∞} ( ln n! - (n ln n - n) - 0.5 ln(2πn) ) * 1? (Через доб. ряд) — возьмём n большое.
status_t calc_gamma(double eps, method_t m, double *out){
	if (!(eps>0) || !out) return ST_ERR_ARGS;
	if (m==M_SERIES){
		long double H=0.0L, prev=0.0L; for (long long n=1;n<200000000LL;++n){ H += 1.0L/n; long double cur = H - logl((long double)n); if ((n&4095)==0){ if (fabsl(cur-prev)<=eps){ *out=(double)cur; return ST_OK; } prev=cur; } } *out=(double)(H - logl(200000000.0L)); return ST_OK;
	} else if (m==M_EQUATION){
		// Численное интегрирование интеграла: γ = ∫_0^1 (1/(1-t) - 1/ln t) dt
		// Заменим t = e^{-u} -> u∈[0,∞): тогда γ = ∫_0^∞ (1/(1-e^{-u}) - 1/u) e^{-у} du
		long double sum=0.0L; long double prev=0.0L; long double h=1e-3L; long double Umax=50.0L; // эксп. затухание
		for (long double u=0.0L; u<Umax; u+=h){
			long double f1 = (1.0L/(1.0L - expl(-u)) - 1.0L/(u>0?u:1.0L)) * expl(-u);
			long double f2_u = u+h; long double f2 = (1.0L/(1.0L - expl(-f2_u)) - 1.0L/(f2_u)) * expl(-f2_u);
			sum += 0.5L*h*(f1+f2);
			if (((long long)(u/h)) % 1024 == 0){ if (fabsl(sum-prev)<=eps){ *out=(double)sum; return ST_OK; } prev=sum; }
		}
		*out=(double)sum; return ST_OK;
	} else { // M_LIMIT через Стирлинг (улучшённый)
		long double prev=0.0L; for (long double n=10.0L; n<5e6L; n*=1.2L){
			// ln n! ≈ sum ln k; посчитаем аккуратно
			long double s=0.0L; for (long long k=1; k<=(long long)n; ++k) s += logl((long double)k);
			long double cur = s - (n*logl(n) - n) - 0.5L*logl(2.0L*3.1415926535897932384626433832795L*n);
			if (fabsl(cur-prev)<=eps){ *out=(double)cur; return ST_OK; } prev=cur;
		}
		*out= (double)prev; return ST_OK;
	}
}