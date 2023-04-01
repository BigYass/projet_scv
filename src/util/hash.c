#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../const.h"
#include "../test/debug.h"
#include "hash.h"

int hashFile (const char* source, const char* dest) {

    char cmd[MAX_BUF_SIZE] = "\0";

    sprintf(cmd, "sha256sum \"%s\" > \"%s\"", source, dest);

    return system(cmd);
}

char* sha256file(const char* file){
    static char template [] = "/tmp/myfileXXXXXX";
    char fname[MAX_BUF_SIZE] = "\0";
    sprintf(fname,  "%s_%s_hash.tmp", template, file);

    mkstemp(fname);

    hashFile(file, fname);

    FILE* f = fopen(fname,  "r");
    char* hash = malloc(sizeof(char) * 65);
    fgets(hash, 64, f);
    hash[64] = '\0';

    return hash;
}

char *hashToPath(const char *hash)
{
    char *path = malloc(sizeof(char) * 66);

    if(path == NULL || hash == NULL){
        err_log(E_ERR, "Erreur chaine de charactère null");
        return NULL;
    }

    path[0] = hash[0];
    path[1] = hash[1];
    path[2] = '/';

    strcpy(path + 3, hash + 2);

    path[65] = '\0';

    return path;
}

static unsigned long
sdbm(unsigned char *str)
{
    unsigned long hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

