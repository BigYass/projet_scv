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
#include "../include/org/my_git.h"

kvp *createKeyVal(const char *key, const char *val)
{
  if(key == NULL || val == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return NULL;
  }

  kvp* k = malloc(sizeof(kvp));

  k->key = strdup(key);
  k->value = strdup(val);

  return k;
}

void freeKeyVal(kvp *kv)
{
  if(kv == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return;
  }

  if(kv->key) free(kv->key);
  if(kv->value) free(kv->value);
  free(kv);
}

void freeCommit(Commit *c)
{
  if(c == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return;
  }

  for(int i = 0; i < c->size; i++)
    if(c->T[i] != NULL) freeKeyVal(c->T[i]);

  free(c->T);
  free(c);
}

char *kvts(kvp *k)
{
  if(k == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
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

  unsigned long hash = sdbm((unsigned char*)key) % c->size;
  while(c->T[hash]){
    hash = (hash + 1) % c->size;
  }

  c->T[hash] = createKeyVal(key, value);

  c->n++;
}

Commit *createCommit(const char *hash)
{
  Commit *c = initCommit();
  commitSet(c, TREE_KEY, hash);
  return c;
}

char *commitGet(Commit *c, const char *key)
{
  if(key == NULL || c == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return NULL;
  }
  unsigned long hash = sdbm((unsigned char*)key) % c->size;
  
  for(int i = 0; i < c->n && c->T[(hash + i) % c->size]; i++)
    if(!strcmp(c->T[(hash + i) % c->size]->key, key))
      return strdup(c->T[(hash + i) % c->size]->value);

  err_logf(E_OK, "Tentative d'accès à une clé non existente %s dans commit 0x%x", key, c);
  return NULL;
}

char *cts(Commit *c)
{
  char *str = malloc(sizeof(char) * MAX_BUF_SIZE * c->n);

  memset(str, 0, sizeof(char) * MAX_BUF_SIZE * c->n);

  for(int i = 0; i < c->size; i++){
    if(c->T[i] != NULL){
      char *s = kvts(c->T[i]);
      strcat(str, s);
      strcat(str, "\n");

      free(s);
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
  if(file == NULL){
    err_log(E_WARN, "Tentative de convertir un fichier avec un nom null..");
    return NULL;
  }

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
  fclose(f);
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
  char fname[MAX_BUF_SIZE] = "myCommitXXXXXX";
  mkstemp(fname);

  ctf(c, fname);

  char *hash = sha256file(fname);
  char *full_path = commitPath(hash);


  cp(full_path, fname);

  free(full_path);
  remove(fname);
  return hash;
}

void restoreCommit(const char *hash)
{
  char *path = commitPath(hash);

  Commit *c = ftc(path);

  if(c == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "ftc", path);
    return;
  }

  char *wt_hash = commitGet(c, TREE_KEY);

  if(wt_hash == NULL){
    err_log(E_ERR, "Le commit ne contien pas \"tree\" ?");
    return;
  }

  char *tree_hash = workTreePath(wt_hash);

  if(tree_hash == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "workTreePath", wt_hash);
    return;
  }

  WorkTree *wt = ftwt(tree_hash);

  if(wt == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "ftwt", tree_hash);
    return;
  }
  
  restoreWorkTree(wt, ".");

  free(wt_hash);
  free(path);
  free(tree_hash);
  freeWorkTree(wt);
  freeCommit(c);
  
}

List *getAllCommits()
{
  List *L = initList();
  List *content = listdir(REFS_DIRECTORY);

  for(Cell *cursor = *content; cursor != NULL; cursor = cursor->next){
    if(cursor->data[0] == '.') continue;

    List *list = branchList(cursor->data);
    Cell *cell = *list;

    while(cell != NULL){
      if(searchList(L, cell->data) == NULL){
        insertFirst(L, buildCell(cell->data));
      }
      cell = cell->next;
    }

    freeList(list);
  }

  freeList(content);

  return L;
}


void createDeletionCommit(const char *branch, List *conflicts, const char* message){
  char *current_branch = getCurrentBranch();
  myGitCheckoutBranch(branch);

  char *branch_hash = getRef(branch);
  if(branch_hash == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "getRef", branch);
    free(current_branch);
    return;
  }

  char *commit_path = commitPath(branch_hash);
  if(commit_path == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "commitPath", branch_hash);
    free(branch_hash);  
    free(current_branch);
    return;
  }
  free(branch_hash);


  Commit *commit = ftc(commit_path);
  if(commit == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "ftc", commit_path);
    free(commit_path);
    free(current_branch);
    return;
  }
  free(commit_path);

  char *wt_hash = commitGet(commit, TREE_KEY);
  freeCommit(commit);
  if(wt_hash == NULL){
    err_log(E_ERR, "Bruh, commit[\"tree\"] = null");
    free(current_branch);
    return;
  }

  char *wt_path = workTreePath(wt_hash);
  if(wt_path == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "workTreePath", wt_hash);
    free(wt_hash);
    free(current_branch);
    return;
  }
  free(wt_hash);
  
  WorkTree *wt = ftwt(wt_path);
  if(wt == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "ftwt", wt_path);
    free(current_branch);
    free(wt_path);
    return;
  }
  free(wt_path);
    

  remove(".add");

  createFile(".add");

  for(int i = 0; i < wt->n; i++)
    if(!searchList(conflicts, wt->tab[i].name))
      myGitAdd(wt->tab[i].name);

  freeWorkTree(wt);

  myGitCommit(branch, message);

  myGitCheckoutBranch(current_branch);

  free(current_branch);
}

