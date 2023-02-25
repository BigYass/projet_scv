#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct cell {
    char* data;
    struct cell* next;
} Cell;


typedef Cell* List;


List* initList() {
    List* l = malloc(sizeof(List));
    return l;
}
Cell* buildCell(char* h) {
    Cell* c = malloc(sizeof(Cell));

    c->data = strdup(h);
    return c;
}

void insertFirst(List *L, Cell* C){
    C->next = *L;
    *L=C;
}

char* ctos(Cell* c){
    return c->data;
}

char* ltos(List* L){
    char* s= malloc(sizeof(char) * 1000);
    s=(*L)->data;
    List cursor = (*L)->next;
    while(cursor){
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


Cell* searchList(List* L, char* str){
    List cursor = (*L)->next;
    while(cursor){
        if(strcmp(cursor->data,str)==0)return cursor;
        cursor=cursor->next;
    }
    return NULL;
}

List* stol(char* s){
    List* l = initList();
    for(char* ptr = s; ptr;){
        ptr = strchr(s, '|');
        if(ptr==NULL)
            ptr = s + strlen(s) - 1;
        int len = ptr - s + 1;

        char* tmp_data = malloc(sizeof(char) * len);
        strncpy(tmp_data, s, len - 1);
        insertFirst(l, buildCell(tmp_data));

        free(tmp_data);
        
    }

    return l;
}

int main(int argc, char const *argv[])
{
    List* l = initList();
    Cell* c1 = buildCell("msg");
    Cell* c2 = buildCell("msg2");
    insertFirst(l,c1);
    insertFirst(l,c2);
    char* s = ltos(l);
    printf("%s\n",s);
    /*List* l2 = stol(s);
    
    Cell * cursor = *l2;

    printf("Premier : %s",cursor->data);
    printf("Premier : %s",cursor->next->data);
    */
    return 0;
}
