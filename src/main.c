#include <stdio.h>
#include <stdlib.h>

#include "const.h"
#include "hash.h"

int main (int argc, char *argv[]) {
    if(argc < 1)
        return 0;

    const char* filename = argv[0];
    
    printf("Hash de %s : ", filename);
    printf("%s\n\n", sha256file(filename));

    return 0;
}


