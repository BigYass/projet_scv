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
    // On crée le fichier en utilisant fopen
    if(file_exists(file)){
        err_logf(E_WARN, E_MSG_FILE_EXIST, file);
        return;
    }
    FILE *f = fopen(file, "w");

    if (f == NULL){
        err_logf(E_WARN, E_MSG_FILE_OPEN, file);
        return;
    }

    // Fermeture du handle
    fclose(f);
}

void ltof(List *L, const char *path)
{
    // On convertit une liste en chaine dans s
    char *s = ltos(L);

    // On le stocke dans un fichier
    FILE *f = fopen(path, "w");

    if(f == NULL){
        err_logf(E_ERR, E_MSG_FILE_OPEN, path);
        free(s);
        return;
    }

    fputs(s, f);

    // Fin de fonction
    fclose(f);
    free(s);
}

List *ftol(const char *path)
{
    // On lit le fichier
    
    FILE *f = fopen(path, "r");

    if (f == NULL)
    {
        err_logf(E_ERR, E_MSG_FILE_OPEN, path);
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

    // La chaine est récupéré, conversion en liste

    List *l = stol(s);
    free(s);

    return l;
}

List *listdir(const char *root_dir)
{
    //Optention du handle sur le dossier
    DIR *dp = opendir(root_dir);
    struct dirent *ep;

    List *l = NULL;

    if (dp != NULL)
    {
        l = initList();
        // Tant qu'on trouve des fichier
        while ((ep = readdir(dp)) != NULL)
        {
            insertFirst(l, buildCell(ep->d_name));
        }
        closedir(dp);
    }
    else
    {
        err_logf(E_ERR, E_MSG_DIR_OPEN, root_dir);
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
    if(to == NULL || from == NULL){
        err_log(E_ERR, E_MSG_PARAM_NULL);
        return;
    }
    //On crée les dossiers parents du fichier tels que "dos/sous-dos/fichier.txt" crée le dossier dos puis sous-dos
    char *cursor = strdup(to);
    char *file = strrchr(cursor, '/');

    if(file) file[0] = '\0';

    char *token = strtok(cursor, "/");


    char current_dir[MAX_BUF_SIZE] = ".";
    while(token != NULL){
        strcat(current_dir, "/");
        strcat(current_dir, token);

        struct stat sb;
        if (stat(current_dir, &sb) == 0) { // Un dossier ou un fichier du meme nom existe
            err_logf(E_OK, "Le nom '%s' est déjà pris par un fichier ou un dossier. Code : %d", current_dir, errno);
        
        } else if (mkdir(current_dir, 0700) != 0) { // créer le dossier avec les droits maximums
            err_logf(E_ERR, E_MSG_MKDIR, current_dir);
            free(cursor);
            return;
        }

    token = strtok(NULL, "/");
    }
    
    free(cursor);

    FILE *dest = fopen(to, "w");

    if (dest == NULL){
        err_logf(E_ERR, E_MSG_FILE_OPEN, to);
        return;
    }

    FILE *source = fopen(from, "r");

    if(source == NULL){
        err_logf(E_ERR, E_MSG_FILE_OPEN, from);
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
        err_log(E_ERR, E_MSG_PARAM_NULL);
        return NULL;
    }

    path[0] = '\0';

    char *hash_path = hashToPath(hash);

    if(hash_path == NULL){
        err_logf(E_ERR, E_MSG_FUNC_NULL, "hashToPath", hash);
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
    if(file == NULL){
        err_log(E_WARN, E_MSG_PARAM_NULL);
        return;
    }
    char *hash = sha256file(file);
    char *path = filePath(hash);

    cp(path, file);

    if(path) free(path);
    if(hash) free(hash);
}
