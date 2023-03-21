#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "const.h"
#include "hash.h"
#include "liste.h"
#include "file.h"


void ltof(List* L, const char* path){
    char *s = ltos(L);
    FILE *f = fopen(path, "w");

    fputs(s, f);
    fclose(f);
    free(s);
}

List* ftol(const char* path){
    List *l;
    char *buf = malloc(sizeof(char) * MAX_BUF_SIZE);
    FILE *f = fopen(path, "r");

    if(f == NULL){
        fprintf(stderr, "%sErreur lors de l'ouverture de %s%s!\n", RED, path, RESET);
        return NULL;
    }

    fgets(buf, MAX_BUF_SIZE, f);

    #ifdef DEBUG
    printf("[DEBUG]buf = %s%s%s\n", RED, buf, RESET);
    #endif

    l = stol(buf);
    free(buf);
    fclose(f);

    return l;
}

List* listdir(const char* root_dir){
    DIR *dp = opendir(root_dir);
    struct dirent *ep;

    List *l = NULL;

    if (dp != NULL)
    {
        l = initList();
        ep = readdir(dp);
        while (ep != NULL)
        {
            insertFirst(l, buildCell(ep->d_name));
            ep = readdir(dp);
        }
    }
    else{
        #ifdef DEBUG
        fprintf(stderr, "L'ouverture du dossier : %s a echoue...\n", root_dir);
        #endif
    }

    return l;
}

bool file_exists(const char *file){
    List *l = listdir("."); //Dossier courant = "."
    Cell *c = searchList(l, file);

    bool result = c != NULL;

    freeList(l);
    return result;
}

void cp(const char *to, const char *from){
    FILE *source = fopen(from, "r");

    if(source != NULL){
        char *buf = malloc(sizeof(char)*MAX_BUF_SIZE);

        char *ptr = NULL;
        do
        {
            char *ptr = strchr(to, '/');
            if(ptr){
                strncpy(buf, to, ptr - to);
                buf[ptr - to] = '\0';

                char cmd[MAX_BUF_SIZE];
                sprintf(cmd, "mkdir %s", buf);
                system(cmd);

                to = ptr + 1;

                ptr = strchr(to, '/');
            }
        } while (ptr > 0);
        
        FILE *dest = fopen(to, "w");

        if(dest == NULL)
        {
            printf("[DEBUG] Erreur lors de la création de %s\"%s\"%s...\n", RED, to, RESET);
        }

        while(fgets(buf, MAX_BUF_SIZE, source))
            fputs(buf, dest);

        free(buf);
        fclose(dest);
    }
    else{
        printf("[DEBUG] Fichier %s\"%s\"%s introuvable...\n", RED, from, RESET);
    }
    fclose(source);
}

char* hashToPath(const char* hash){
    char* path = malloc(sizeof(char) * 66);

    path[0] = hash[0];
    path[1] = hash[1];
    path[2] = '/';

    strcpy(path + 3, hash + 2);

    path[65] = '\0';

    return path;
}

void blobFile(const char* file){
    char *path = malloc(sizeof(char)*MAX_BUF_SIZE);
    
    char *hash_path = hashToPath(sha256file(file));
    strcat(path, hash_path);


    strcat(path, file);

    cp(path, file);

    free(hash_path);
    free(path);
}
