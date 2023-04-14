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
  int err = mkdir(REFS_DIRECTORY, 0777);
  if(err){
    err_logf(E_WARN, E_MSG_MKDIR, REFS_DIRECTORY);
    return;
  }

  FILE *f = fopen(REFS_DIRECTORY "/master", "w");
  fclose(f);
  f = fopen(REFS_DIRECTORY  "/HEAD", "w");
  fclose(f);
}

void createUpdateRef(const char *ref_name, const char *hash)
{
  char path[MAX_BUF_SIZE] = REFS_DIRECTORY "/";

  if(ref_name == NULL){
    err_log(E_ERR, E_MSG_PARAM_NULL);
    return;
  }

  strcat(path, ref_name);
  FILE *f = fopen(path, "w");

  if(f == NULL){
    err_logf(E_ERR, E_MSG_FILE_OPEN, path);
  }

  fputs(hash, f);

  fclose(f);
}

void deleteRef(const char *ref_name)
{
  char buff[MAX_BUF_SIZE] = REFS_DIRECTORY "/";
  strcat(buff, ref_name);

  if(remove(buff))
    err_logf(E_WARN, E_MSG_FILE_SUPPR, buff);
}

char *getRef(const char *ref_name)
{
  char buff[MAX_BUF_SIZE] = REFS_DIRECTORY "/";
  strcat(buff, ref_name);

  FILE *f = fopen(buff, "r");

  if(f == NULL){
    err_logf(E_ERR, E_MSG_FILE_OPEN, buff);
    return NULL;
  }

  char *s = malloc(sizeof(char) * MAX_BUF_SIZE);
  memset(s, 0, MAX_BUF_SIZE);

  fgets(s, MAX_BUF_SIZE, f);
  fclose(f);
  return s;
}
