#include <malloc.h>
#include <string.h>
#include <stdio.h>
#include <sys/stat.h>

#include "../include/const.h"
#include "../include/test/debug.h"
#include "../include/util/file.h"
#include "../include/org/refs.h"

void initRefs()
{
  int err = mkdir(".refs", 0777);
  if(err){
    err_log(E_WARN, "Problème lors de la création de .refs");
    return;
  }

  FILE *f = fopen(".refs/master", "w");
  fclose(f);
  f = fopen(".refs/HEAD", "w");
}

void createUpdateRef(const char *ref_name, const char *hash)
{
  char *path = ".refs/";
  strcat(path, ref_name);
  FILE *f = fopen(path, "w");

  if(f == NULL){
    err_logf(E_ERR, "Problème lors de l'ouverture du fichier %s", path);
  }

  fputs(hash, f);

  fclose(f);
}

void deleteRef(const char *ref_name)
{
  char buff[MAX_BUF_SIZE] = ".refs/";
  strcat(buff, ref_name);

  if(remove(buff))
    err_logf(E_WARN, "Problème lors de la supression du fichier %s", buff);
}

char *getRef(const char *ref_name)
{
  char buff[MAX_BUF_SIZE] = ".refs/";
  strcat(buff, ref_name);

  FILE *f = fopen(buff, "r");

  if(f == NULL){
    err_logf(E_ERR, "Problème lors de la lecture du fichier %s", buff);
    return NULL;
  }

  char *s = malloc(sizeof(char) * MAX_BUF_SIZE);
  fgets(s, MAX_BUF_SIZE, f);
  fclose(f);
  return s;
}
