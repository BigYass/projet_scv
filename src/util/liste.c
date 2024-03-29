#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "../include/const.h"
#include "../include/test/debug.h"
#include "../include/util/liste.h"
#include "../include/util/hash.h"

List *initList()
{
    List *l = calloc(1, sizeof(List));

    if (l == NULL)
    {
        err_log(E_ERR, "Erreur lors de l'allocation initiale d'une liste. Pas assez de RAM ?");
        return NULL;
    }

    *l = NULL;
    return l;
}

Cell *buildCell(const char *h)
{
    if(h == NULL){
        err_log(E_WARN, E_MSG_PARAM_NULL);
        return NULL; 
    }

    Cell *c = calloc(1, sizeof(Cell));

    if (c == NULL)
    {
        err_log(E_ERR, E_MSG_MALLOC_NULL);
        return NULL;
    }

    c->data = strdup(h);
    c->next = NULL;
    return c;
}

void insertFirst(List *L, Cell *C)
{
    if (C == NULL || L == NULL)
        err_log(E_ERR, E_MSG_PARAM_NULL);

    C->next = *L;
    *L = C;
}

void freeList(List *L)
{
    if (L == NULL)
    {
        err_log(E_WARN, E_MSG_PARAM_NULL);
        return;
    }

    Cell *cursor = *L;
    while(cursor){
        Cell *next = cursor->next;
        freeCell(cursor);
        cursor = next;
    }
    
    free(L);
}

void freeCell(Cell *c)
{
    if (c == NULL)
    {
        err_log(E_WARN, E_MSG_PARAM_NULL);
        return;
    }

    free(c->data);
    free(c);
}

char *ctos(Cell *c)
{
    if (c == NULL)
    {
        err_log(E_ERR, E_MSG_PARAM_NULL);
        return "";
    }
    return c->data;
}

char *ltos(List *L)
{
    size_t size = MAX_BUF_SIZE;
    char *s = malloc(sizeof(char) * size);
    if (s == NULL || L == NULL || *L == NULL)
    {
        err_log(E_ERR, E_MSG_PARAM_NULL);
        return "";
    }

    memset(s, 0, size);

    strcat(s, (*L)->data);
    Cell *cursor = NULL;
    cursor = (*L)->next;
    while (cursor != NULL)
    {
        // Si la chaine de charactère n'est pas assez longue on double la capacité
        if (size < strlen(s) + strlen(cursor->data) + 1)
        {
            size *= 2;
            s = realloc(s, sizeof(char) * size);
            if (s == NULL)
            {
                err_log(E_ERR, E_MSG_ALLOC_NULL);
            }
        }
        strcat(s, "|");
        strcat(s, ctos(cursor));
        cursor = cursor->next;
    }

    // On réaloue la chaine pour économisé mémoire. A optimisé
    char *result = strdup(s);
    free(s);
    return result;
}

Cell *listGet(List *L, int i)
{
    if (L == NULL)
    {
        err_log(E_WARN, E_MSG_PARAM_NULL);
        return NULL;
    }
    Cell *cursor = *L;
    for (; i > 0; i--)
    {
        if (!cursor)
            return NULL;
        cursor = cursor->next;
    }

    if (cursor == NULL)
        err_logf(E_WARN, "Index out of range (%d)", i);

    return cursor;
}

Cell *searchList(List *L, const char *str)
{
    if (L == NULL || str == NULL)
    {
        err_log(E_WARN, E_MSG_PARAM_NULL);
        return NULL;
    }
    List cursor = *L;
    while (cursor)
    {
        if (strcmp(cursor->data, str) == 0)
            return cursor;
        cursor = cursor->next;
    }
    return NULL;
}

List *stol(const char *s)
{
    List *l = initList();
    char *s_buf = strdup(s);

    char *token = strtok(s_buf, "|"); // On cherche le prochain mot
    // Tant que s n'est pas NULL et non égale à '\0'
    do
    {
        Cell *c = buildCell(token);
        insertFirst(l, c);
    } while ((token = strtok(NULL, "|")) != NULL);

    free(s_buf);

    return l;
}

List *filterList(List *L, const char *pattern)
{
    List *filtered = initList();

    int len = strlen(pattern);
    for (Cell *cursor = *L; cursor != NULL; cursor = cursor->next)
    {
        if (strncmp(cursor->data, pattern, len))
        {
            insertFirst(filtered, cursor);
        }
    }

    return filtered;
}

int sizeList(List *L)
{
    if (L == NULL)
    {
        err_log(E_WARN, E_MSG_PARAM_NULL);
        return 0;
    }

    int len = 0;

    for (Cell *cursor = *L; cursor != NULL; cursor = cursor->next)
        len++;

    return len;
}