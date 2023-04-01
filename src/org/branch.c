#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "branch.h"
#include "../const.h"
#include "../org/refs.h"
#include "../org/commit.h"
#include "../test/debug.h"
#include "../util/file.h"
#include "../util/hash.h"

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
  List *refs = listdir(".refs");
  int result = searchList(refs, branch) != NULL;
  freeList(refs);
  return result;
}

void createBranch(const char *branch)
{
  char *hash = getRef("HEAD");
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
  char *path = hashToPath(commit_hash);
  strcat(path, ".c");
  Commit *c = ftc(path);

  while(c != NULL){
    char *msg = commitGet(c, "message");
    char *prev = commitGet(c, "predecessor");
    if(msg) printf("%s => \"%s\"\n", commit_hash, msg);
    else printf("%s\n", commit_hash);

    if(prev){
      commit_hash = prev;
      
      path = hashToPath(commit_hash);
      strcat(path, ".c");

      freeCommit(c);
      c = ftc(path);
    }
    else {
      c = NULL;
    }
  }
}

List *branchList(const char *branch)
{
  List *L = initList();

  char *hash = getRef(branch);
  char *path = hashToPath(hash);
  strcat(path, ".c");
  Commit *c = ftc(path);

  while(c != NULL){
    insertFirst(L, buildCell(hash));
    char *prev = commitGet(c, "predecessor");

    if(prev){
      hash = prev;

      path = hashToPath(hash);
      strcat(path, ".c");

      freeCommit(c);
      c = ftc(path);
    }
    else {
      c = NULL;
    }
  }

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
