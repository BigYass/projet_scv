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
    *L=C;
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
        if(size < strlen(s) + strlen(cursor->data) + 1){
            size *= 2; s = realloc(s, sizeof(char) * size);
        }
        strcat(s, "|");
        strcat (s,ctos(cursor));
        cursor = cursor->next;
    }

    return strdup(s);
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

    char *buffer = malloc(sizeof(char) * MAX_BUF_SIZE);

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

    free(buffer);
    return l;
}
