#include <stdio.h>
#include "task7.h"

int main(int argc, char **argv){
    if (argc!=3){ 
        fprintf(stderr,"Usage: %s <in> <out>\n", argv[0]); 
        return ST_ERR_ARGS; 
    }
    return task7_process(argv[1], argv[2]);
}
