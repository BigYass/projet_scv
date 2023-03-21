#include "const.h"
#include "liste.h"
#include "hash.h"

List* initList() {
    List* l = malloc(sizeof(List));
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

    if(f == NULL){
        fprintf(stderr, "%sErreur lors de l'ouverture de %s!\n", path);
        return NULL;
    }

    fgets(buf, MAX_BUF_SIZE, f);

    #ifdef DEBUG
    printf("[DEBUG]buf = %s%s%s\n", RED, buf, RESET);
    #endif

    l = stol(buf);
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

        fclose(dest);
    }
    else{
        printf("[DEBUG] Fichier %s\"%s\"%s introuvable...\n", RED, from, RESET);
    }
    fclose(source);
}

char* hashToPath(const char* hash){
    char* path = malloc(sizeof(char) * (strlen(hash) + 1));

    path[0] = hash[0];
    path[1] = hash[1];
    path[2] = '/';

    strcpy(path + 3, hash + 2);

    path[65] = '\0';

    return path;
}

void blobFile(const char* file){
    char path[MAX_BUF_SIZE];
    
    char *hash_path = hashToPath(sha256file(file));
    strcat(path, hash_path);

    free(hash_path);

    strcat(path, file);

    cp(path, file);
}
