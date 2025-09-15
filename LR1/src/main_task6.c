#include <stdio.h>
#include <string.h>
#include "task6.h"


static void usage(const char *p){ fprintf(stderr, "Usage: %s <eps> <integral_id> <a> <b>\n integral_id: 1->f1, 2->f2 (временные)\n", p); }


int main(int argc, char **argv){
	if (argc!=5){ usage(argv[0]); return ST_ERR_ARGS; }
	double eps=0,a=0,b=0; int id=0; if (sscanf(argv[1],"%lf",&eps)!=1||eps<=0) return ST_ERR_ARGS; if (sscanf(argv[2],"%d",&id)!=1) return ST_ERR_ARGS; if (sscanf(argv[3],"%lf",&a)!=1) return ST_ERR_ARGS; if (sscanf(argv[4],"%lf",&b)!=1) return ST_ERR_ARGS;
	func_t f=NULL; if (id==1) f=f1; else if (id==2) f=f2; else { usage(argv[0]); return ST_ERR_ARGS; }
	double val = integrate_trapezoid(f,a,b,eps); printf("%.15g\n", val); return 0;
}


