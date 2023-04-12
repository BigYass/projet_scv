#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "../include/const.h"
#include "../include/test/debug.h"
#include "../include/util/liste.h"
#include "../include/util/hash.h"



List* initList() {
    List* l = malloc(sizeof(List));

    if(l == NULL){
        err_log(E_ERR, "Erreur lors de l'allocation initiale d'une liste. Pas assez de RAM ?");
        return NULL;
    }

    *l = NULL;
    return l;
}

Cell* buildCell(const char* h) {
    Cell* c = malloc(sizeof(Cell));

    if(c == NULL){
        err_log(E_ERR, "Erreur lors de l'allocation initiale d'une cellule de liste. Pas assez RAM ?");
        return NULL;
    }

    c->data = strdup(h);
    c->next = NULL;
    return c;
}

void insertFirst(List *L, Cell* C){
    if(C == NULL || L == NULL)
        err_logf(E_ERR, "Erreur: C = (0x%x) // L = (0x%x)", L, C);

    C->next = *L;
    *L = C;
}

void freeList(List *L){
    if(L == NULL) {
        err_log(E_WARN, "Tentative de libérée une liste null...");
        return;
    }

    Cell *cursor = *L;

    while(cursor != NULL){
        Cell *prev = cursor;
        cursor = cursor->next;
        freeCell(prev);
    }
    free(L);
}

void freeCell(Cell *c){
    if(c == NULL){
        err_log(E_WARN, "Tentative de libéré une cellule null");
        return;
    }

    if(c->data) free(c->data);
    free(c);
}

char* ctos(Cell* c){
    if(c == NULL){
        err_log(E_ERR, "Tentative de conversion d'une cellule null...");
        return "";
    }
    return c->data;
}

char* ltos(List* L){
    size_t size = MAX_BUF_SIZE;
    char *s = malloc(sizeof(char) * size);
    if(s == NULL || L == NULL || *L == NULL) {
        err_logf(E_ERR, "Erreur: s = \"%s\"\n\tL = 0x%x", s, L);
        return "";
    }

    memset(s, 0, size);

    strcat(s, (*L)->data);
    Cell *cursor = NULL;
    cursor = (*L)->next;
    while(cursor != NULL){
        //Si la chaine de charactère n'est pas assez longue on double la capacité
        if(size < strlen(s) + strlen(cursor->data) + 1){ 
            size *= 2; s = realloc(s, sizeof(char) * size);
            if(s == NULL){
                err_log(E_ERR, "Erreur lors de l'allocation de s");
            }
        }
        strcat(s, "|");
        strcat (s,ctos(cursor));
        cursor = cursor->next;
    }

    //On réaloue la chaine pour économisé mémoire. A optimisé
    char* result = strdup(s);
    free(s);
    return result;
}

Cell* listGet(List* L, int i){
    if(L == NULL){
        err_log(E_WARN, "Tentative de parcours d'une liste vide");
        return NULL;
    }
    Cell *cursor = *L;
    for(;i > 0; i--){
        if (!cursor) return NULL;
        cursor = cursor->next;
    }

    if(cursor == NULL)
        err_logf(E_ERR, "Index out of range (%d)", i);

    return cursor;
}

Cell* searchList(List* L, const char* str){
    if(L == NULL || str == NULL){
        err_logf(E_WARN, "Erreur: Paramètre nul (s = \"%s\", L = 0x%x)", str, L);
        return NULL;
    }
    List cursor = *L;
    while(cursor){
        if(strcmp(cursor->data,str)==0)return cursor;
        cursor=cursor->next;
    }
    return NULL;
}

List* stol(const char* s){
    List* l = initList();
    char* s_buf = strdup(s);

    char *token = strtok(s_buf, "|"); //On cherche le prochain mot
    //Tant que s n'est pas NULL et non égale à '\0'
    do {
        Cell *c = buildCell(token);
        insertFirst(l, c); 
    }
    while((token = strtok(NULL, "|")) != NULL);

    free(s_buf);

    return l;
}

List *filterList(List * L, const char *pattern)
{
    List *filtered = initList();

    int len = strlen(pattern);
    for(Cell *cursor = *L; cursor != NULL; cursor = cursor->next){
        if(strncmp(cursor->data, pattern, len)){
            insertFirst(filtered, cursor);
        }
    }

    return filtered;
}

int sizeList(List *L){
    int len = 0;

    for(Cell *cursor = *L; cursor != NULL; cursor = cursor->next) 
        len++;

    return len;
}