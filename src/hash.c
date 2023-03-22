#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"
#include "hash.h"
#include "debug.h"

int hashFile (const char* source, const char* dest) {

    char cmd[MAX_BUF_SIZE] = "\0";

    sprintf(cmd, "sha256sum \"%s\" > \"%s\"", source, dest);

    return system(cmd);
}

char* sha256file(const char* file){

    char fname[MAX_BUF_SIZE] = "\0";
    sprintf(fname,  "%s/%s_hash.tmp", TMP_DIRECTORY, file);

    hashFile(file, fname);

    FILE* f = fopen(fname,  "r");

    char* hash = malloc(sizeof(char) * 65);

    fgets(hash, 64, f);

    hash[64] = '\0';

    return hash;
}
