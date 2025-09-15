#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "task5.h"

static int is_latin_letter(int ch){ 
    ch=(unsigned char)ch; 
    return (ch>='A'&&ch<='Z')||(ch>='a'&&ch<='z'); 
}

status_t task5_run(const char *flag, const char *in_path, const char *out_path_opt){
    if (!flag || !in_path) return ST_ERR_ARGS;
    char op=0; 
    int named=0; // named output?
    if ((flag[0]=='-'||flag[0]=='/') && flag[1]=='n'){ 
        named=1; 
        op=flag[2]; 
    }
    else if (flag[0]=='-'||flag[0]=='/'){ 
        op=flag[1]; 
    }
    else return ST_ERR_ARGS;

    char out_path_buf[1024]; 
    const char *outp = out_path_opt;
    if (!named){
        // prefix out_
        const char *fname=in_path; 
        const char *slash=strrchr(in_path,'/'); 
        const char *bslash=strrchr(in_path,'\\'); 
        if (slash && (!bslash || slash>bslash)) fname=slash+1; 
        else if (bslash) fname=bslash+1;
        snprintf(out_path_buf, sizeof(out_path_buf), "out_%s", fname);
        outp = out_path_buf;
    } else if (!outp) return ST_ERR_ARGS;

    FILE *fi=fopen(in_path,"rb"); 
    if(!fi) return ST_ERR_IO;
    FILE *fo=fopen(outp,"wb"); 
    if(!fo){ fclose(fi); return ST_ERR_IO; }

    if (op=='d'){ // remove digits
        int ch; 
        while ((ch=fgetc(fi))!=EOF){ 
            if (!(ch>='0'&&ch<='9')) fputc(ch,fo); 
        }
    } else if (op=='i'){ // count latin letters per line
        int ch; 
        long cnt=0; 
        while ((ch=fgetc(fi))!=EOF){ 
            if (ch=='\n'){ 
                fprintf(fo, "%ld\n", cnt); 
                cnt=0; 
            } else { 
                if (is_latin_letter(ch)) ++cnt; 
            } 
        } 
        if (cnt>0) fprintf(fo, "%ld\n", cnt);
    } else if (op=='s'){ // count non [latin|digit|space]
        int ch; 
        long cnt=0; 
        while ((ch=fgetc(fi))!=EOF){ 
            if (ch=='\n'){ 
                fprintf(fo, "%ld\n", cnt); 
                cnt=0; 
            } else { 
                if (!is_latin_letter(ch) && !isdigit((unsigned char)ch) && !isspace((unsigned char)ch)) ++cnt; 
            } 
        } 
        if (cnt>0) fprintf(fo, "%ld\n", cnt);
    } else if (op=='a'){ // replace non-digits with ASCII hex
        int ch; 
        while ((ch=fgetc(fi))!=EOF){ 
            if (ch>='0'&&ch<='9'){ 
                fputc(ch,fo);
            } else { 
                fprintf(fo, "%02X", (unsigned char)ch); 
            } 
        }
    } else { 
        fclose(fi); 
        fclose(fo); 
        return ST_ERR_ARGS; 
    }

    fclose(fi); 
    fclose(fo); 
    return ST_OK;
}
