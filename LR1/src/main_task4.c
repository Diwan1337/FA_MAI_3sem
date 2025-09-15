#include <stdio.h>
#include <string.h>
#include "task4.h"


static void usage(const char *p){
	fprintf(stderr,
	"Usage: %s <const> <method> <eps>\n"
	" const: e|pi|ln2|sqrt2|gamma\n"
	" method: series|equation|limit\n",
	p);
}


static method_t parse_m(const char *s){
	if (strcmp(s,"series")==0) return M_SERIES;
	if (strcmp(s,"equation")==0) return M_EQUATION;
	return M_LIMIT;
}


int main(int argc, char **argv){
	if (argc!=4){ usage(argv[0]); return ST_ERR_ARGS; }
	const char *c=argv[1]; method_t m=parse_m(argv[2]); double eps=0; if (sscanf(argv[3],"%lf",&eps)!=1 || eps<=0) return ST_ERR_ARGS;
	double out=0.0; status_t st=ST_OK;
	if (strcmp(c,"e")==0) st=calc_e(eps,m,&out);
	else if (strcmp(c,"pi")==0) st=calc_pi(eps,m,&out);
	else if (strcmp(c,"ln2")==0) st=calc_ln2(eps,m,&out);
	else if (strcmp(c,"sqrt2")==0) st=calc_sqrt2(eps,m,&out);
	else if (strcmp(c,"gamma")==0) st=calc_gamma(eps,m,&out);
	else { usage(argv[0]); return ST_ERR_ARGS; }
	if (st!=ST_OK) return st; printf("%.15g\n", out); return 0;
}


