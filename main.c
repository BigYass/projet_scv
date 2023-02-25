#include <stdio.h>
#include <stdlib.h>
#include <openssl/sha.h>

#define MAX_BUF_SIZE 100

char* sha256file(char* file);

int main () {
    printf("Hash de main.c : %s\n\n", sha256file("main.c"));
}

int hashFile (char* source, char* dest) {
    char cmd[MAX_BUF_SIZE];

    sprintf(cmd, "sha256sum %s > %s", source, dest);

    return system(cmd);
}

char* sha256file(char* file){
    static char template[] = "/tmp/temporaireXXXXXX";
    char fname[MAX_BUF_SIZE];
    strcpy(fname,template);
    mkstemp(fname);

    hashFile(file, fname);

    FILE* f = fopen(fname,  "r");

    char* hash = malloc(sizeof(char) * 65);

    fgets(hash, 64, f);

    return hash;
}

