#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/org/workfile.h"
#include "../include/const.h"
#include "../include/test/debug.h"
#include "../include/util/hash.h"
#include "../include/util/file.h"
#include "../include/org/commit.h"
#include "../include/org/branch.h"
#include "../include/org/refs.h"
#include "../include/org/my_git.h"

WorkFile *createWorkFile(const char *name)
{
  if(name == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return NULL;
  }
  // calloc mets à 0 la zone contrairemet à malloc, plus besoin de le faire manuellement
  WorkFile *wf = calloc(sizeof(WorkFile), 1); 
  
  // memset(wf, 0, sizeof(WorkFile));

  wf->name = strdup(name);
  wf->hash = NULL;
  wf->mode = 0;

  return wf;
}

void freeWorkFile(WorkFile *wf)
{
  if(wf == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return;
  }

  // En théorie name n'est jamais null
  if(wf->hash) free(wf->hash);
  if(wf->name) free(wf->name);
  free(wf);
}

char *wfts(WorkFile *wf)
{
  char *s = calloc(sizeof(char), MAX_BUF_SIZE);

  sprintf(s, "%s\t%s\t%d", wf->name, wf->hash, wf->mode);

  return s;
}

WorkFile *stwf(const char *ch)
{
  char name[MAX_BUF_SIZE] = "\0", hash[MAX_BUF_SIZE] = "\0";
  int mode;

  sscanf(ch, "%s\t%s\t%d", name, hash, &mode);
  WorkFile *wf = createWorkFile(name);

  wf->hash = strdup(hash);
  wf->mode = mode;

  return wf;
}

WorkTree *initWorkTree()
{
  WorkTree *wt = malloc(sizeof(WorkTree));
  wt->tab = calloc(sizeof(WorkFile), WORKTREE_SIZE);
  //memset(wt->tab, 0, WORKTREE_SIZE * sizeof(WorkFile));

  wt->size = WORKTREE_SIZE;
  wt->n = 0;

  return wt;
}

void freeWorkTree(WorkTree *wt){
  if(wt == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return;
  }
  for(int i = 0; i < wt->size; i++){
    if(wt->tab[i].name != NULL) free(wt->tab[i].name);
    if(wt->tab[i].hash != NULL) free(wt->tab[i].hash);
  }
  free(wt->tab);
  free(wt);
}

int inWorkTree(WorkTree *wt, const char *name)
{
  for (int i = 0; i < wt->n; i++)
    if (!strcmp(wt->tab[i].name, name))
      return i;
  return -1;
}

int appendWorkTree(WorkTree *wt, const char *name, const char *hash, int mode)
{
  if (name == NULL || !strcmp(name, ""))
  {
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return -3;
  }
  if (inWorkTree(wt, name) >= 0)
  {
    err_logf(E_WARN, E_MSG_STR_EXIST, name);
    return -1;
  }
  if (wt->size <= wt->n)
  {
    err_log(E_WARN, "Ajout impossible, le WortTree est plein");
    return -2;
  }
  
  wt->tab[wt->n].mode = mode;
  wt->tab[wt->n].name = strdup(name);
  wt->tab[wt->n++].hash = (hash != NULL) ? strdup(hash) : NULL; 

  return 0;
}

char *wtts(WorkTree *wt)
{
  size_t size = MAX_BUF_SIZE * wt->size;
  char *s = malloc(sizeof(char) * size);

  memset(s, 0, size);

  for (int i = 0; i < wt->n; i++)
  {
    char *buf = wfts(&(wt->tab[i]));

    // réallocation en cas d'espace insuffisante
    if (size <= strlen(buf) + strlen(s))
    {
      size <<= 1;
      s = realloc(s, sizeof(char) * size);
    }

    strcat(s, buf);
    strcat(s, "\n");
    free(buf);
  }

  return s;
}

WorkTree *stwt(char *s)
{
  WorkTree *wt = initWorkTree();
  char *token = strtok(s, "\n");

  while (token != NULL)
  {
    WorkFile *wf = stwf(token);

    appendWorkTree(wt, wf->name, wf->hash, wf->mode);

    freeWorkFile(wf);

    token = strtok(NULL, "\n");
  }

  return wt;
}

// ==== MANIPULATION DE FICHIER ====

int wttf(WorkTree *wt, const char *file)
{
  FILE *f = fopen(file, "w");
  if (f == NULL)
  {
    err_logf(E_ERR, E_MSG_FILE_OPEN, file);
    return -1;
  }

  char *s = wtts(wt);
  fputs(s, f);
  free(s);
  fclose(f);
  return 0;
}

WorkTree *ftwt(const char *file)
{
  FILE *f = fopen(file, "r");

  if (f == NULL)
  {
    err_logf(E_ERR, E_MSG_FILE_OPEN, file);
    return NULL;
  }

  size_t size = MAX_BUF_SIZE * WORKTREE_SIZE;
  char *s = malloc(sizeof(char) * size), buf[MAX_BUF_SIZE];

  memset(s, 0, size);

  while (fgets(buf, MAX_BUF_SIZE, f))
  {
    if (size <= strlen(s) + strlen(buf))
    {
      size *= 2;
      s = realloc(s, size);
    }
    strcat(s, buf);
  }

  fclose(f);

  WorkTree *wt = stwt(s);
  free(s);
  return wt;
}

char* workTreePath(const char* hash){
  if(hash == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return NULL;
  }

  char *hash_path = hashToPath(hash);

  if(hash_path == NULL){
    err_logf(E_WARN, E_MSG_FUNC_NULL, "hashToPath", hash);
    return NULL;
  }

  char *path = malloc(sizeof(char) * MAX_BUF_SIZE);
  memset(path, 0, MAX_BUF_SIZE); 

  strcat(path, TMP_DIRECTORY);
  strcat(path, "/");
  strcat(path, hash_path);
  strcat(path, ".t");

  free(hash_path);

  return path;
}

char *blobWorkTree(WorkTree *wt)
{
  char fname[MAX_BUF_SIZE] = "myWorkTreeXXXXXX";
  mkstemp(fname);

  int err = 0;

  if ((err = wttf(wt, fname)))
  {
    err_logf(E_ERR, "La fonction wwtf a retourné %d", err);
    return "";
  }


  char full_path[MAX_BUF_SIZE] = {0}; //Chaine vide

  char *hash = sha256file(fname);
  char *hash_path = hashToPath(hash);

  // Construction du chemin d'accès temporaire
  strcat(full_path, TMP_DIRECTORY);
  strcat(full_path, "/");
  strcat(full_path, hash_path);
  strcat(full_path, ".t");
  
  cp(full_path, fname);
  remove(fname);
  free(hash_path);
  return hash;
}

char *saveWorkTree(WorkTree *wt, char *path)
{
  for (int i = 0; i < wt->n; i++)
  {
    char *full_path = malloc(sizeof(char) * (strlen(path) + strlen(wt->tab[i].name) + 2));

    memset(full_path, 0, sizeof(char) * (strlen(path) + strlen(wt->tab[i].name) + 2));

    strcat(full_path, path);
    if(!(wt->tab[i].name[0] && wt->tab[i].name[0] == '/')) strcat(full_path, "/");
    strcat(full_path, wt->tab[i].name);

    if (file_exists(full_path))
    {
      blobFile(full_path);
      if(wt->tab[i].hash) free(wt->tab[i].hash);
      wt->tab[i].hash = sha256file(full_path);
      wt->tab[i].mode = getChmod(full_path);
    }
    else
    {
      WorkTree *wt_rep = initWorkTree();
      List *L = listdir(full_path); 
      if(L == NULL){
        err_logf(E_ERR, E_MSG_FUNC_NULL, "listdir", full_path);
        return NULL;
      }

      for (Cell *cursor = *L; cursor != NULL; cursor = cursor->next)
      {
        if (ctos(cursor)[0] == '.')
          continue;
        appendWorkTree(wt_rep, ctos(cursor), NULL, 0);
      }

      wt->tab[i].hash = saveWorkTree(wt_rep, full_path);
      wt->tab[i].mode = getChmod(full_path);

      freeWorkTree(wt_rep);
      freeList(L);
    }
  
    free(full_path);
  }
  return blobWorkTree(wt);
}

int isWorkTree(char *hash)
{
  char *file_path = filePath(hash);
  if (file_exists(file_path))
  {
    free(file_path);
    return 0;
  }
  strcat(file_path, ".t");
  if (file_exists(file_path))
  {
    free(file_path);
    return 1;
  }
  free(file_path);
  return -1;
}

void restoreWorkTree(WorkTree *wt, char *path)
{
  for (int i = 0; i<wt -> n; i++)
  {
    size_t size = strlen(path) + strlen(wt->tab[i].name) + 2;
    char *full_path = malloc(sizeof(char) * size);
    memset(full_path, 0, size);

    strcat(full_path, path);
    strcat(full_path, "/");
    strcat(full_path, wt->tab[i].name);

    char *hash = wt->tab[i].hash;
    if (isWorkTree(hash) == 0)
    { // si c’est un fichier
      char *copyPath = filePath(hash);
      cp(full_path, copyPath);
      setMode(getChmod(copyPath), full_path); free(copyPath);
    }
    else if (isWorkTree(hash) == 1)
    { // si c’est un repertoire
      char *copyPath = workTreePath(hash);
      WorkTree *nwt = ftwt(copyPath);
      restoreWorkTree(nwt, full_path);
      setMode(getChmod(copyPath), full_path);
      freeWorkTree(nwt);
    }
    free(full_path);
  }
}

WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts){
  WorkTree *wt = initWorkTree();

  // Recherche des conflits
  if(wt1) for(int i = 0; i < wt1->n; i++)
    for(int j = 0; j < wt2->n; j++)
      if(!strcmp(wt1->tab[i].name, wt2->tab[j].name))
        insertFirst(*conflicts, buildCell(wt1->tab[i].name));
  

  // On rajoute les éléments de wt1
  if(wt1) for(int i = 0; i < wt1->n; i++)
      if(searchList(*conflicts, wt1->tab[i].name))
        appendWorkTree(wt, wt1->tab[i].name, wt1->tab[i].hash, wt1->tab[i].mode);

  // On rajoute les éléments de wt2
  if(wt2) for(int i = 0; i < wt2->n; i++)
      if(searchList(*conflicts, wt2->tab[i].name))
        appendWorkTree(wt, wt2->tab[i].name, wt2->tab[i].hash, wt2->tab[i].mode);

  return wt;
}
