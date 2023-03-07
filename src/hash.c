#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"

/**
 * @brief Fait la somme de hashage de l'algorithme sha256 et stock le résultat dans dans un fichier
 * 
 * @param source Nom du fichier source qui sera haché
 * @param dest Nom du fichier de sortie où le résultat sera stocké. Si le fichier n'existe pas, le fichier sera crée
 * @return int Retourne le résultat de l'appel de la fonction 
 * 
 */
int hashFile (const char* source, const char* dest) {
    #ifdef DEBUG
    printf("[DEBUG]Début de la fonction hashFile\n");
    #endif

    char cmd[MAX_BUF_SIZE];

    #ifndef WINDOWS
    sprintf(cmd, "sha256sum \"%s\" > \"%s\"", source, dest);
    #else
    sprintf(cmd, "CertUtil -hashfile \"%s\" SHA256 | find /i /v \"%s\" > \"%s\"", source, source, dest);

    FILE *result = fopen(dest, "r");

    #ifdef DEBUG
    printf("[DEBUG]Detection du resultat\n");
    #endif

    fgets(cmd, MAX_BUF_SIZE, result);

    #ifdef DEBUG
    printf("[DEBUG]Hash : %s!\n", cmd);
    #endif

    fgets(cmd, MAX_BUF_SIZE, result);

    #ifdef DEBUG
    printf("[DEBUG]Hash : %s!\n", cmd);
    #endif

    fgets(cmd, MAX_BUF_SIZE, result);

    #ifdef DEBUG
    printf("[DEBUG]Hash : %s!\n", cmd);
    #endif
    
    fclose(result);

    remove(dest);

    result = fopen(dest, "w");

    fwrite(cmd, 65, 1, result);

    fclose(result);

    #endif

    #ifdef DEBUG
    printf("[DEBUG]Fin de fonction hashFile\n");
    #endif

    return system(cmd);
}

char* sha256file(const char* file){
    #ifdef DEBUG
    printf("[DEBUG]Début de la fonction sha256file\n");
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
