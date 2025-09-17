#include <stdio.h>
#include <string.h>
#include "task5.h"
#include <ctype.h>

static int str_ieq(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        ++a; ++b;
    }
    return *a == '\0' && *b == '\0';
}

static int flag_is(const char* s, const char* one_letter) {
    if (!s || !*s) return 0;
    if (s[0] != '-' && s[0] != '/') return 0;
    ++s;
    if (*s == '-') ++s;
    return s[0] && !s[1] && tolower((unsigned char)s[0]) == tolower((unsigned char)one_letter[0]);
}


static void usage(const char *p){
    fprintf(stderr, "Usage: %s <-d|-i|-s|-a | -nd|-ni|-ns|-na> <in> [out_if_n]\n", p);
}


int main(int argc, char **argv){
    if (argc<3){ usage(argv[0]); return ST_ERR_ARGS; }
    const char *flag=argv[1]; const char *in=argv[2]; const char *out = ( (flag[0]=='-'||flag[0]=='/') && tolower((unsigned char)flag[1])=='n' ? (argc>=4?argv[3]:NULL) : NULL );
    status_t st = task5_run(flag,in,out); if (st!=ST_OK) return st; return 0;
}
