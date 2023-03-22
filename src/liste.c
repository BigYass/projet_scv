#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "const.h"
#include "liste.h"
#include "hash.h"



List* initList() {
    List* l = malloc(sizeof(List));
    *l = NULL;
    return l;
}

Cell* buildCell(const char* h) {
    Cell* c = malloc(sizeof(Cell));

    c->data = strdup(h);
    c->next = NULL;
    return c;
}

void insertFirst(List *L, Cell* C){
    C->next = *L;
    *L = C;
}

void freeList(List *L){
    if(L != NULL) return;

    Cell *cursor = *L;
    free(L);

    Cell *prev = cursor;
    cursor = cursor->next;
    while(cursor){
        if(prev) free(prev);
        cursor = cursor->next;
    }
    if(prev) free(prev);
}

char* ctos(Cell* c){
    return strdup(c->data);
}

char* ltos(List* L){
    int size = MAX_BUF_SIZE;
    char *s = malloc(sizeof(char) * size);
    if(s == NULL || L == NULL || *L == NULL) return NULL;

    memset(s, 0, size);

    strcat(s, (*L)->data);
    Cell *cursor = NULL;
    cursor = (*L)->next;
    while(cursor != NULL){
        //Si la chaine de charactère n'est pas assez longue on double la capacité
        if(size < strlen(s) + strlen(cursor->data) + 1){ 
            size *= 2; s = realloc(s, sizeof(char) * size);
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
    memset(buffer, 0, MAX_BUF_SIZE);

    //Tant que s n'est pas NULL et non égale à '\0'
    while(s && s[0]){
        char *ptr = strchr(s, '|'); //On cherche le prochain séparateur
        if (ptr == NULL) //Si on ne trouve plus de séparateur on quite la boucle et on ajoute la dernière cellule en bas
            break;
        
        //On place la chaine de charactère du début de s jusqu'au '|' trouvé
        strncpy(buffer, s, ptr - s);
        buffer[ptr - s] = '\0';
        
        insertFirst(l, buildCell(buffer));

        //On reprend s après le '|' trouvé. Par exemple "abc|def" -> "def"
        s = ptr + 1;
    }

    if(s && s[0]) //Cas où la chaine de charactère ne finis pas par LISTE_SEPARATOR
        insertFirst(l, buildCell(s));

    return l;
}
