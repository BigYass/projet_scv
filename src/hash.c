#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"

int hashFile (const char* source, const char* dest) {
    #ifdef DEBUG
    printf("[DEBUG]Début de la fonction hashFile\n");
    #endif

    char cmd[MAX_BUF_SIZE];

    sprintf(cmd, "sha256sum \"%s\" > \"%s\"", source, dest);

    #ifdef DEBUG
    printf("[DEBUG]Fin de fonction %shashFile%s\n", GREEN, RESET);
    #endif

    return system(cmd);
}

char* sha256file(const char* file){
    #ifdef DEBUG
    printf("[DEBUG]Début de la fonction %ssha256file%s\n", GREEN, RESET);
    #endif

    char fname[MAX_BUF_SIZE];
    sprintf(fname,  ".tmp/%s_hash.tmp", file);

    hashFile(file, fname);

    FILE* f = fopen(fname,  "r");

    char* hash = malloc(sizeof(char) * 65);

    fgets(hash, 64, f);

    hash[64] = '\0';

    return hash;
}
