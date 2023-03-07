#ifndef LISTE_LIB
#define LISTE_LIB

typedef struct cell {
    char* data;
    struct cell* next;
} Cell;


typedef Cell* List;

List* initList();
Cell* buildCell(char* h);
void insertFirst(List *L, Cell* C);
char* ctos(Cell* c);
char* ltos(List* L);
Cell* listGet(List* L, int i);
Cell* searchList(List* L, char* str);

#endif