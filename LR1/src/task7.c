#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "task7.h"
#include "base_utils.h"

status_t task7_process(const char *in_path, const char *out_path){
    FILE *fi=fopen(in_path,"rb"); 
    if(!fi) return ST_ERR_IO; 
    FILE *fo=fopen(out_path,"wb"); 
    if(!fo){ fclose(fi); return ST_ERR_IO; }
    
    char buf[4096];
    while (fscanf(fi, "%4095s", buf)==1){
        // Исходное без ведущих нулей (кроме нуля)
        int base=2; 
        status_t st = minimal_valid_base(buf,&base); 
        if (st!=ST_OK){ 
            fprintf(fo, "%s INVALID\n", buf); 
            continue; 
        }
        
        // Уберём ведущие нули для печати
        const char *p=buf; 
        int neg = (*p=='-'); 
        if (neg||*p=='+') ++p; 
        size_t len=0; 
        while (p[len] && !isspace((unsigned char)p[len])) ++len; 
        size_t newlen = trim_leading_zeros(&p, len);
        if (newlen==0){ p="0"; }
        
        // to int64 base10
        long long val=0; 
        st = to_int64_base(buf, base, &val);
        if (st==ST_ERR_OVERFLOW){ 
            fprintf(fo, "%s %d OVERFLOW\n", p, base); 
        }
        else if (st!=ST_OK){ 
            fprintf(fo, "%s %d INVALID\n", p, base); 
        }
        else {
            // напечатаем val в десятичной
            char dec[128]; 
            int pos=127; 
            dec[pos]='\0'; 
            long long tmp = val; 
            int neg2 = (tmp<0); 
            if (neg2) tmp = -tmp; 
            if (tmp==0){ dec[--pos]='0'; }
            while (tmp>0 && pos>0){ 
                int d = (int)(tmp % 10); 
                tmp/=10; 
                dec[--pos] = '0'+d; 
            }
            if (neg2) dec[--pos]='-';
            fprintf(fo, "%s %d %s\n", p, base, &dec[pos]);
        }
    }
    fclose(fi); 
    fclose(fo); 
    return ST_OK;
}
