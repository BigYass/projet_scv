#include <malloc.h>
#include <string.h>
#include <stdlib.h>

#include "../include/org/commit.h"
#include "../include/const.h"
#include "../include/util/hash.h"
#include "../include/util/file.h"
#include "../include/test/debug.h"
#include "../include/org/workfile.h"
#include "../include/org/refs.h"
#include "../include/org/branch.h"


kvp *createKeyVal(const char *key, const char *val)
{
  kvp* k = malloc(sizeof(kvp));

  k->key = (char *) strdup(key);
  k->value = (char *) strdup(val);

  return k;
}

void freeKeyVal(kvp *kv)
{
  if(kv == NULL){
    err_log(E_WARN, "Tentative de libéré une paire null");
    return;
  }

  if(kv->key) free(kv->key);
  if(kv->value) free(kv->value);
  free(kv);

  kv = NULL;
}

void freeCommit(Commit *c)
{
  for(int i = 0; i < c->size; i++)
    if(c->T[i]) freeKeyVal(c->T[i]);

  free(c);

  c = NULL;
}

char *kvts(kvp *k)
{
  if(k == NULL){
    err_log(E_WARN, "Tentative de conversion d'une paire NULL");
    return NULL;
  }
  char *s = malloc(sizeof(char) * MAX_BUF_SIZE);

  sprintf(s, "%s : %s", k->key ? k->key : "", k->value ? k->value : "");

  return s;
}

kvp *stkv(const char *str)
{
  char *key = malloc(sizeof(char) * MAX_BUF_SIZE), *val = malloc(sizeof(char) * MAX_BUF_SIZE);

  memset(key, 0, MAX_BUF_SIZE);
  memset(val, 0, MAX_BUF_SIZE);

  sscanf(str, "%s : %s", key, val);

  kvp *result = createKeyVal(key, val);

  free(key); free(val);

  return result;
}

Commit *initCommit()
{
  Commit *commit = malloc(sizeof(Commit));

  commit->n = 0;
  commit->size = COMMIT_SIZE;
  commit->T = malloc(sizeof(kvp) * COMMIT_SIZE);

  memset(commit->T, 0, COMMIT_SIZE * sizeof(kvp));

  return commit;
}

void commitSet(Commit *c, const char *key, const char *value)
{
  if(key == NULL){
    err_log(E_WARN, "Tentative d'insertion d'une clé nul dans un commit");
    return;
  }

  if(c->n >= c->size){
    err_log(E_ERR, "Tentative d'insertion d'une clé dans un commit plein...");
    return;
  }

  int hash = sdbm((unsigned char*)key) % c->size;
  while(c->T[hash]){
    hash = (hash + 1) % c->size;
  }

  c->T[hash] = createKeyVal(key, value);

  c->n++;
}

Commit *createCommit(const char *hash)
{
  Commit *c = initCommit();
  commitSet(c, "tree", hash);
  return c;
}

char *commitGet(Commit *c, const char *key)
{
  int hash = sdbm((unsigned char*)key) % c->size;
  
  for(int i = 0; i < c->n && c->T[(hash + i) % c->size]; i++)
    if(!strcmp(c->T[(hash + i) % c->size]->key, key))
      return c->T[(hash + i) % c->size]->value;

  err_logf(E_OK, "Tentative d'accès à une clé non existente %s dans commit 0x%x", key, c);
  return NULL;
}

char *cts(Commit *c)
{
  char *str = malloc(sizeof(char) * MAX_BUF_SIZE * c->n);

  memset(str, 0, sizeof(char) * MAX_BUF_SIZE * c->n);

  for(int i = 0; i < c->size; i++){
    if(c->T[i] != NULL){
      strcat(str, kvts(c->T[i]));
      strcat(str, "\n");
    }
  }

  return str;
}

Commit *stc(char *s)
{
  Commit *c = initCommit();

  char *token = strtok(s, "\n");

  while(token){
    kvp* pair = stkv(token);

    commitSet(c, pair->key, pair->value);
    freeKeyVal(pair);
    token = strtok(NULL, "\n");
  }


  return c;  
}

void ctf(Commit *c, const char *file)
{
  FILE *f = fopen(file, "w");
  if(f == NULL){
    err_logf(E_ERR, "Problème lors de la création %s", file);
    return;
  }

  char *s = cts(c);
  fputs(s, f);
  free(s);
  fclose(f);
}

Commit *ftc(const char *file)
{
  FILE *f = fopen(file, "r");

  if(f == NULL){
    err_logf(E_ERR, "Erreur lors de l'ouverture du fichier %s", file);
    return NULL;
  }

  size_t size = sizeof(char) * MAX_BUF_SIZE;
  char *s = malloc(size);

  memset(s, 0, size);

  char buf[MAX_BUF_SIZE];
  while(fgets(buf, MAX_BUF_SIZE, f)){
    if(strlen(buf) + strlen(s) >= size){
      size *= 2; s = realloc(s, size);
    }
    strcat(s, buf);
  }

  Commit *c = stc(s);
  free(s);
  return c;
}

char* commitPath(const char* hash){

  if(hash == NULL){
    err_log(E_WARN, "Tentative de conversion avec un hash null");
    return NULL;
  }

  char *hash_path = hashToPath(hash);

  if(hash_path == NULL){
    err_logf(E_WARN, YELLOW "hashToPath(\"%s\") " RESET " a renvoyé NULL", hash);
    return NULL;
  }

  char *path = malloc(sizeof(char) * MAX_BUF_SIZE);
  memset(path, 0, MAX_BUF_SIZE); 

  strcat(path, TMP_DIRECTORY);
  strcat(path, "/");
  strcat(path, hash_path);
  strcat(path, ".c");

  free(hash_path);

  return path;
}

char *blobCommit(Commit *c)
{
  char fname[MAX_BUF_SIZE] = "myWorkTreeXXXXXX";
  mkstemp(fname);

  ctf(c, fname);

  char *hash = sha256file(fname);
  char *full_path = commitPath(hash);


  cp(full_path, fname);

  remove(fname);
  return hash;
}

void restoreCommit(const char *hash)
{
  char *path = commitPath(hash);

  Commit *c = ftc(path);

  if(c == NULL){
    err_log(E_ERR, "La fonction ftc a renvoyé NULL");
    return;
  }

  char *wt_hash = commitGet(c, "tree");

  if(wt_hash == NULL){
    err_log(E_ERR, "Le commit de contien pas \"tree\" ?");
    return;
  }

  char *tree_hash = workTreePath(wt_hash);

  if(wt_hash == NULL){
    err_logf(E_ERR, "Impossible de convertir" YELLOW "%s" RESET " en chemin d'accès...", wt_hash);
    return;
  }

  WorkTree *wt = ftwt(tree_hash);

  if(wt == NULL){
    err_logf(E_ERR, "Problème de conversion en WorkTree du fichier %s", tree_hash);
    return;
  }
  
  restoreWorkTree(wt, ".");

  free(path);
  free(tree_hash);
  freeCommit(c);
  
}

void myGitCheckoutCommit(const char* pattern)
{
  List *L = getAllCommits();
  List *filtered_list = filterList(L, pattern);

  int len = sizeList(filtered_list);
  if (len == 1){
    char *commit_hash = (*L)->data;;
    createUpdateRef("HEAD", commit_hash);
    restoreCommit(commit_hash);
  }
  else if (len == 0){
    printf("Aucun hash ne commence par "YELLOW"%s"RESET"...\n", pattern);
  }
  else {
    printf("Plusieurs hash commencent par "YELLOW"%s"RESET" :\n", pattern);
    for(Cell *c = *filtered_list; c; c = c->next)
      printf(" -> %s\n", c->data);
  }

  freeList(L);
  freeList(filtered_list);
}
