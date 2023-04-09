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

  k->key = (char *) key;
  k->value = (char *) val;

  return k;
}

void freeKeyVal(kvp *kv)
{
  free(kv->key);
  free(kv->value);
  free(kv);
}

void freeCommit(Commit *c)
{
  for(int i = 0; i < c->size; i++)
    if(c->T[i]) freeKeyVal(c->T[i]);

  free(c);
}

char *kvts(kvp *k)
{
  char *s = malloc(sizeof(char) * MAX_BUF_SIZE);

  sprintf(s, "%s : %s", k->key, k->value);

  return s;
}

kvp *stkv(const char *str)
{
  char key[MAX_BUF_SIZE] = "\0", val[MAX_BUF_SIZE] = "\0";
  sscanf(str, "%s : %s", key, val);

  return createKeyVal(key, val);
}

Commit *initCommit()
{
  Commit *commit = malloc(sizeof(Commit));

  commit->n = 0;
  commit->size = COMMIT_SIZE;
  commit->T = malloc(sizeof(kvp) * COMMIT_SIZE);

  return commit;
}

void commitSet(Commit *c, const char *key, const char *value)
{
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

char *blobCommit(Commit *c)
{
  char fname[MAX_BUF_SIZE] = ".tmp/myWorkTreeXXXXXX";
  mkstemp(fname);

  ctf(c, fname);

  char *hash = sha256file(fname);
  char *path = hashToPath(hash);

  strcat(path, ".c");

  cp(path, fname);
  return hash;
}

void restoreCommit(const char *hash)
{
  char *path = hashToPath(hash);
  Commit *c = ftc(path);

  char *tree_hash = strcat(hashToPath(commitGet(c, "tree")), ".t");
  WorkTree *wt = ftwt(tree_hash);
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
