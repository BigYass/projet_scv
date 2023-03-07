#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>

#include "liste.h"
#include "const.h"

List* initList() {
    List* l = malloc(sizeof(List));
    return l;
}

Cell* buildCell(const char* h) {
    Cell* c = malloc(sizeof(Cell));

    c->data = strdup(h);
    return c;
}

void insertFirst(List *L, Cell* C){
    C->next = *L;
    *L=C;
}

void freeList(List *L){
    Cell *cursor = *L;

    free(L);

    Cell *prev = cursor;
    while(cursor){
        cursor = cursor->next;
        free(prev);
    }
    free(prev);
}

char* ctos(Cell* c){
    return c->data;
}

char* ltos(List* L){
    char *s = malloc(sizeof(char) * MAX_BUF_SIZE);
    strcat(s, (*L)->data);
    Cell *cursor = (*L)->next;
    while(cursor != NULL){
        strcat(s, "|");
        strcat (s,ctos(cursor));
        cursor = cursor->next;
    }

    return s;
}

Cell* listGet(List* L, int i){
    Cell *cursor = *L;
    for(;i > 0; i--){
        if (!cursor) return NULL;
        cursor = cursor->next;
    }

    return cursor;
}

Cell* searchList(List* L, const char* str){
    List cursor = (*L)->next;
    while(cursor){
        if(strcmp(cursor->data,str)==0)return cursor;
        cursor=cursor->next;
    }
    return NULL;
}

List* stol(const char* s){
    List* l = initList();

    char buffer[MAX_BUF_SIZE];

    //Tant que s n'est pas NULL et non égale à '\0'
    while(s && s[0]){
        char *ptr = strchr(s, '|');
        if (ptr == NULL) //Si on ne trouve plus de séparateur on quite la boucle et on ajoute la dernière cellule en bas
            break;
        
        strncpy(buffer, s, ptr - s);
        buffer[ptr - s] = '\0';
        
        insertFirst(l, buildCell(buffer));

        s = ptr + 1;
    }

    if(s && s[0]) //Cas où la chapine de charactère ne finis par '|'
        insertFirst(l, buildCell(s));
    return l;
}

char* list_string(List* L){
    char *buf = malloc(sizeof(char) * MAX_BUF_SIZE), delim[5] = {0};
    for(Cell *cursor = *L; cursor != NULL; strcpy(delim, " -> \0")){
        strcat(buf, delim);
        strcat(buf, cursor->data);
        cursor = cursor->next;
    }

    return buf;
}


void ltof(List* L, const char* path){
    char *s = ltos(L);
    FILE *f = fopen(path, "w");

    fputs(s, f);
    fclose(f);
    free(s);
}

List* ftol(const char* path){
    List *l;
    char buf[MAX_BUF_SIZE];
    FILE *f = fopen(path, "r");

    fgets(buf, MAX_BUF_SIZE, f);

    fclose(f);

    l = stol(buf);

    return l;
}

List* listdir(const char* root_dir){
    DIR *dp = opendir(root_dir);
    struct dirent *ep;

    List *l = NULL;

    if (dp != NULL)
    {
        l = initList();
        while ((ep = readdir(dp)) != NULL)
        {
            insertFirst(l, buildCell(ep->d_name));
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
        char buf[MAX_BUF_SIZE];
        FILE *dest = fopen(to, "w");

        while(fgets(buf, MAX_BUF_SIZE, source))
            fputs(buf, dest);

        fclose(source);
        fclose(dest);
    }
    else{
        #ifdef DEBUG
        printf("[DEBUG] Fichier %s\"%s\"%s introuvable...\n", RED, from, RESET);
        #endif
    }
}

char* hashToPath(const char* hash){
    char* path = malloc(sizeof(char) * (strlen(hash) + 1));

    path[0] = hash[0];
    path[1] = hash[1];
    path[2] = '/';

    strcpy(path + 3, hash + 2);

    return path;
}

void blobFile(const char* file){
    static const char* tmp_path = ".tmp/";
    
}
