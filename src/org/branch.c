#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "../include/org/branch.h"
#include "../include/const.h"
#include "../include/org/refs.h"
#include "../include/org/commit.h"
#include "../include/org/workfile.h"
#include "../include/test/debug.h"
#include "../include/util/file.h"
#include "../include/util/hash.h"

void initBranch()
{
  FILE *f = fopen(".current_branch", "w");

  if(f == NULL){
    err_log(E_ERR, "Problème lors de l'ouverture du fichier .current_branch");
    return;
  }

  fputs("master", f);
  fclose(f);
}

int branchExists(const char *branch)
{
  List *refs = listdir(".refs/");
  int result = searchList(refs, branch) != NULL;
  freeList(refs);
  return result;
}

void createBranch(const char *branch)
{
  if(branch == NULL){
    return;
  }

  char *hash = getRef("HEAD");

  if(hash == NULL){
    err_log(E_ERR, "Chaine de charactère NULL...");
    return;
  }

  createUpdateRef(branch, hash);
  free(hash);
}

char *getCurrentBranch()
{
  FILE *f = fopen(".current_branch", "r");
  char *buff = malloc(sizeof(char) * MAX_BUF_SIZE);

  fgets(buff, MAX_BUF_SIZE, f);
  fclose(f);

  return buff;
}

void printBranch(const char *branch)
{
  char *commit_hash = getRef(branch);
  char *path = commitPath(commit_hash);

  Commit *c = ftc(path);

  if(path) free(path);

  while(c != NULL){
    char *msg = commitGet(c, "message");
    char *prev = commitGet(c, "predecessor");
    if(msg) {
      printf("%s => \"%s\"\n", commit_hash, msg);
      free(msg);
    }

    else printf("%s\n", commit_hash);

    if(prev){
      commit_hash = strdup(prev);
      free(prev);
      path = commitPath(commit_hash);

      if(commit_hash) free(commit_hash);
      freeCommit(c);
      c = ftc(path);
      if (c == NULL){
        err_log(E_ERR, "ftc a renvoyé null..");
        if(path) free(path);
        break;
      }

      if(path) free(path);
    }
    else {
      freeCommit(c);
      c = NULL;
    }
  }
}

List *branchList(const char *branch)
{
  List *L = initList();

  char *hash = getRef(branch);
  if(hash == NULL){
    err_logf(E_ERR, "getRef(\"%s\") a renvoye NULL", branch);
    return NULL;
  }

  char *path = commitPath(hash);
  if(path == NULL){
    err_logf(E_ERR, "commitPath(\"%s\") a renvoye NULL", hash);
    return NULL;
  }

  Commit *c = ftc(path);
  if(c == NULL){
    err_logf(E_ERR, "La conversion de (\"%s\") a échoué", path);
    return NULL;
  }

  while(c != NULL){
    insertFirst(L, buildCell(hash));
    char *prev = commitGet(c, "predecessor");

    if(path) free(path);
    if(hash) free(hash);
    if(prev){
      hash = strdup(prev); // On duplique pour facilité la libération de mémoire
      path = commitPath(hash);

      free(prev);
      freeCommit(c);
      c = ftc(path);
    }
    else {
      freeCommit(c);
      c = NULL;
    }
  }

  if(path) free(path);

  return L;
}

List *getAllCommits()
{
  List *L = initList();
  List *content = listdir(".refs");

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
void myGitCheckoutBranch(const char *branch)
{
  FILE *f = fopen(".current_branch", "w");
  fprintf(f, "%s", branch);
  fclose(f);

  char* hash_commit = getRef(branch);

  if(hash_commit == NULL){
    err_logf(E_ERR, "Le hash de %s a retourné null...", branch);
    return;
  }

  createUpdateRef("HEAD", hash_commit);
  restoreCommit(hash_commit);

  free(hash_commit);
}
