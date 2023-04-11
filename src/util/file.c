#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <dirent.h>

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "../include/const.h"
#include "../include/util/hash.h"
#include "../include/util/liste.h"
#include "../include/util/file.h"
#include "../include/test/debug.h"

void createFile(const char *file)
{
    if(file_exists(file)){
        err_logf(E_WARN, "Le fichier %s existe déjà", file);
        return;
    }
    FILE *f = fopen(file, "w");

    if (f == NULL){
        err_logf(E_WARN, "Problème lors de la création de %s", file);
        return;
    }

    fclose(f);
}

void ltof(List *L, const char *path)
{
    char *s = ltos(L);
    FILE *f = fopen(path, "w");

    if(f == NULL){
        err_logf(E_ERR, "Erreur lors de l'ouverture du fichier %s", path);
        free(s);
        return;
    }

    fputs(s, f);
    fclose(f);
    free(s);
}

List *ftol(const char *path)
{
    
    FILE *f = fopen(path, "r");

    if (f == NULL)
    {
        err_logf(E_ERR, "%sErreur lors de l'ouverture de %s%s!\n", RED, path, RESET);
        return NULL;
    }
    
    // Récupération du contenu du fichier dans s
    char *buf[MAX_BUF_SIZE];

    size_t size = sizeof(buf);
    char *s = malloc(size); s[0] = '\0';

    while(fgets((char *)buf, sizeof(buf) - 1, f) != NULL){
        if(size < strlen(s) + strlen((char *)buf - 1)){
            size *= 2; 
            s = realloc(s, size);
        }

        strcat(s, (char *)buf);
    }
    fclose(f);

    List *l = stol(s);

    return l;
}

List *listdir(const char *root_dir)
{
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
        closedir(dp);
    }
    else
    {
        err_logf(E_ERR, "L'ouverture du dossier : %s a echoue...\n", root_dir);
    }

    return l;
}

bool file_exists(const char *file)
{
    struct stat buf;
    return (stat(file, &buf) == 0);
}

void cp(const char *to, const char *from)
{
    //On crée les dossiers parents du fichier tels que "dos/sous-dos/fichier.txt" crée le dossier dos puis sous-dos
    char *cursor = strdup(to);
    char *file = strrchr(cursor, '/');

    if(file) file[0] = '\0'; // Séparation du nom du fichier des dossiers parents

    char *token = strtok(cursor, "/");


    char current_dir[MAX_BUF_SIZE] = ".";
    while(token != NULL){
        strcat(current_dir, "/");
        strcat(current_dir, token);

        struct stat sb;
        if (stat(current_dir, &sb) == 0) { // Un dossier ou un fichier du meme nom existe
            err_logf(E_WARN, "Le nom '%s' est déjà pris par un fichier ou un dossier. Code : %d\n", current_dir, errno);
        
        } else if (mkdir(current_dir, 0700) != 0) { // créer le dossier avec les droits maximums
            err_logf(E_ERR, "Impossible de créer le dossier '%s'\n", current_dir);
            free(cursor);
            return;
        }

    token = strtok(NULL, "/");
    }
    
    free(cursor);

    FILE *dest = fopen(to, "w");

    if (dest == NULL){
        err_logf(E_ERR, "Erreur lors de la création de %s\"%s\"%s...\n", RED, to, RESET);
        return;
    }

    FILE *source = fopen(from, "r");

    if(source == NULL){
        err_logf(E_ERR, "Erreur lors l'ouverture de %s", from);
        fclose(dest);
        return;
    }

    char buf[MAX_BUF_SIZE];
    while (fgets(buf, MAX_BUF_SIZE, source))
        fputs(buf, dest);

    fclose(dest);
    fclose(source);
}

char *filePath(const char *hash){
    char *path = malloc(sizeof(char) * MAX_BUF_SIZE);

    if(path == NULL || hash == NULL){
        err_log(E_ERR, "Null exeption");
        return NULL;
    }

    path[0] = '\0';

    char *hash_path = hashToPath(hash);

    if(hash_path == NULL){
        err_logf(E_ERR, "hashToPath a retourné NULL avec %s", hash);
        free(path);
        return NULL;
    }

    strcat(path, TMP_DIRECTORY);
    strcat(path, "/");
    strcat(path, hash_path);

    free(hash_path);

    return path;
}

void blobFile(const char *file)
{
    char *path = filePath(file);

    cp(path, file);

    free(path);
}
