#include <stdio.h>
#include <string.h>
#include "task5.h"


static void usage(const char *p){
    fprintf(stderr, "Usage: %s <-d|-i|-s|-a | -nd|-ni|-ns|-na> <in> [out_if_n]\n", p);
}


int main(int argc, char **argv){
    if (argc<3){ usage(argv[0]); return ST_ERR_ARGS; }
    const char *flag=argv[1]; const char *in=argv[2]; const char *out = ( (flag[0]=='-'||flag[0]=='/') && flag[1]=='n' ? (argc>=4?argv[3]:NULL) : NULL );
    status_t st = task5_run(flag,in,out); if (st!=ST_OK) return st; return 0;
}
