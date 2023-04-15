#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/const.h"
#include "../include/test/debug.h"
#include "../include/util/hash.h"

int hashFile (const char* source, const char* dest) {

    char cmd[MAX_BUF_SIZE] = "\0"; 

    sprintf(cmd, "sha256sum \"%s\" > \"%s\"", source, dest);

    return system(cmd);
}

char* sha256file(const char* file){
    char fname[MAX_BUF_SIZE] = "\0";
    sprintf(fname,  "%s%s%s.hash", TMP_DIRECTORY, file[0] == '/' ? "" : "/", file);


    hashFile(file, fname);

    FILE* f = fopen(fname,  "r");

    if(f == NULL){
        err_logf(E_ERR, E_MSG_FILE_OPEN, fname);
        return NULL;
    }
    
    char* hash = malloc(sizeof(char) * 65);
    memset(hash, 0, 65 * sizeof(char));

    fgets(hash, 64, f);
    hash[64] = '\0';

    fclose(f);

    remove(fname);

    return hash;
}

char *hashToPath(const char *hash)
{
    char *path = malloc(sizeof(char) * 66);

    if(path == NULL || hash == NULL){
        err_log(E_ERR, E_MSG_PARAM_NULL);
        return NULL;
    }

    path[0] = hash[0];
    path[1] = hash[1];
    path[2] = '/';

    strcpy(path + 3, hash + 2);

    path[65] = '\0';

    return path;
}

unsigned long
sdbm(unsigned char *str)
{
    unsigned long hash = 0;
    int c;

    while ((c = *str++))
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

