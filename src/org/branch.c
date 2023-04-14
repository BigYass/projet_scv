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
    err_logf(E_ERR, E_MSG_FILE_OPEN, ".current_branch");
    return;
  }

  fputs("master", f);
  fclose(f);
}

int branchExists(const char *branch)
{
  List *refs = listdir(REFS_DIRECTORY "/");
  int result = searchList(refs, branch) != NULL;
  freeList(refs);
  return result;
}

void createBranch(const char *branch)
{
  if(branch == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return;
  }

  char *hash = getRef("HEAD");

  if(hash == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "getRef", "HEAD");
    return;
  }

  createUpdateRef(branch, hash);
  free(hash);
}

char *getCurrentBranch()
{
  FILE *f = fopen(".current_branch", "r");
  if(f == NULL){
    err_logf(E_ERR, E_MSG_FILE_OPEN, ".current_branch");
    return NULL;
  }

  char *buff = malloc(sizeof(char) * MAX_BUF_SIZE);

  fgets(buff, MAX_BUF_SIZE, f);
  fclose(f);

  return buff;
}

void printBranch(const char *branch)
{
  if(branch == NULL){
    err_log(E_WARN, E_MSG_PARAM_NULL);
    return;
  }

  char *commit_hash = getRef(branch);
  char *path = commitPath(commit_hash);

  Commit *c = ftc(path);

  if(path) free(path);

  while(c != NULL){
    char *msg = commitGet(c, MESSAGE_KEY);
    if(msg) {
      printf("%s => \"%s\"\n", commit_hash, msg);
      free(msg);
    }

    else printf("%s\n", commit_hash);

    
    if(commit_hash) free(commit_hash);
    commit_hash = commitGet(c, PREDECESSOR_KEY);
    freeCommit(c);

    path = commitPath(commit_hash);

    c = ftc(path);

    if(path) free(path);
  }

  if(commit_hash) free(commit_hash);
}

List *branchList(const char *branch)
{
  List *L = initList();

  char *hash = getRef(branch);
  if(hash == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "getRef", branch);
    return NULL;
  }

  char *path = commitPath(hash);
  if(path == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "commitPath", hash);
    free(hash);
    return NULL;
  }

  Commit *c = ftc(path);
  if(c == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "ftc", path);
    free(hash); free(path);
    return NULL;
  }

  while(c != NULL){
    insertFirst(L, buildCell(hash));

    if(path) free(path);
    if(hash) free(hash);
    hash = commitGet(c, PREDECESSOR_KEY); // On duplique pour facilité la libération de mémoire
    path = commitPath(hash);

    freeCommit(c);
    c = ftc(path);
  }

  if(path) free(path);
  if(hash) free(hash);

  return L;
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
void myGitCheckoutBranch(const char *branch)
{
  FILE *f = fopen(".current_branch", "w");
  fprintf(f, "%s", branch);
  fclose(f);

  char* hash_commit = getRef(branch);

  if(hash_commit == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "getRef", branch);
    return;
  }

  createUpdateRef("HEAD", hash_commit);
  restoreCommit(hash_commit);

  free(hash_commit);
}
