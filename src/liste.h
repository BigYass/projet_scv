#ifndef LISTE_LIB
#define LISTE_LIB

typedef struct cell {
    char* data;
    struct cell* next;
} Cell;

typedef Cell* List;

//Gestion de liste
List* initList();
Cell* buildCell(const char* h);

void insertFirst(List *L, Cell* C);
void freeList(List *L);

char* ctos(Cell* c);
char* ltos(List* L);
char* list_string(List* L);

Cell* listGet(List* L, int i);
Cell* searchList(List* L, const char* str);

List* stol(const char* s);

void ltof(List* L, const char* path);
List* ftol(const char* path);

//Gestion de fichier
List* listdir(const char* root_dir);
bool file_exists(const char *file);

/**
 * @brief Copie le fichier {@codefrom} vers le fichier {@codeto} ligne par ligne
 * 
 * @param to Nom du fichier destinataire
 * @param from Nom du fichier source, si il n'existe pas rien ne se passe
 */
void cp(const char *to, const char *from); 
char* hashToPath(const char* hash);
void blobFile(const char* file); 


#endif