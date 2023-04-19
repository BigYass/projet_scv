#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "../include/org/my_git.h"
#include "../include/org/workfile.h"
#include "../include/org/refs.h"
#include "../include/org/commit.h"
#include "../include/org/branch.h"
#include "../include/const.h"
#include "../include/util/file.h"
#include "../include/util/hash.h"
#include "../include/test/debug.h"

void myGitAdd(const char *file)
{
  WorkTree *wt = NULL;
  if(!file_exists(".add")){
    createFile(".add");
    wt = initWorkTree();
  }
  else {
    wt = ftwt(".add");
  }

  if(file_exists(file)){
    char *hash = sha256file(file);
    appendWorkTree(wt, file, hash, 0);
    wttf(wt, ".add");
    
    free(hash);
  }
  else {
    err_logf(E_ERR, E_MSG_FILE_EXIST, file);
  }
  freeWorkTree(wt);
}

void myGitCommit(const char *branch_name, const char *message)
{
  if(!file_exists(REFS_DIRECTORY)){
    err_log(E_WARN, "Il faut d'abord initialiser les références du projets");
    return;
  }

  char path[MAX_BUF_SIZE] = REFS_DIRECTORY "/";
  strcat(path, branch_name);
  if(!file_exists(path)){
    err_logf(E_WARN, "La branche %s n'existe pas", path);
    return;
  }

  char *last_hash = getRef(branch_name);
  char *head_hash = getRef("HEAD");

  bool is_different = strcmp(last_hash, head_hash);

  free(head_hash);

  if(is_different){
    err_logf(E_ERR, "HEAD doit pointer sur le dernier commit de la branche %s", branch_name);
    return;
  }

  WorkTree *wt = ftwt(".add");

  if(wt == NULL){
    err_log(E_ERR, "Il faut crée le fichier .add");
    return;
  }

  char *hash = saveWorkTree(wt, ".");
  Commit *c = createCommit(hash);
  free(hash);

  if (strlen(last_hash) > 0){
    commitSet(c, PREDECESSOR_KEY, last_hash);
  }
  if(message != NULL){
    commitSet(c, MESSAGE_KEY, message); // commit["message"] = message;
  }
  free(last_hash);

  char *commit_hash = blobCommit(c);
  createUpdateRef(branch_name, commit_hash);
  createUpdateRef("HEAD", commit_hash);

  free(commit_hash);
  freeCommit(c);
  freeWorkTree(wt);
  
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


List* merge(const char* remote_branch, const char* message){
  if(remote_branch == NULL){
    err_log(E_ERR, E_MSG_PARAM_NULL);
    return NULL;
  }

  char *current_branch_name = getCurrentBranch();
  char *current_branch = getRef(current_branch_name);

  char *current_commit_path = commitPath(current_branch);
  if(current_commit_path == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "commitPath", current_branch);
  }
  
  char *remote_branch_ref = getRef(remote_branch);
  char *remote_commit_path = commitPath(remote_branch_ref);
  free(remote_branch_ref);
  if(current_commit_path == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "commitPath", remote_branch);
  }

  Commit *current_commit = ftc(current_commit_path);
  if(current_commit == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "ftc", current_commit_path);
    return NULL;
  }

  Commit *remote_commit = ftc(remote_commit_path);
  if(current_commit == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "ftc", remote_commit_path);
    return NULL;
  }

  char *current_wt_hash = commitGet(current_commit, TREE_KEY);
  if(current_wt_hash == NULL){
    err_logf(E_ERR, "Impossible de trouver tree dans le commit %s", current_commit_path);
  }

  char *remote_wt_hash = commitGet(remote_commit, TREE_KEY);
  if(remote_wt_hash == NULL){
    err_logf(E_ERR, "Impossible de trouver tree dans le commit %s", remote_commit_path);
  }

  char *current_wt_path = workTreePath(current_wt_hash);
  WorkTree *current_wt = ftwt(current_wt_path);
  if(current_wt == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "ftwt", current_wt_hash);
  }


  char *remote_wt_path = workTreePath(remote_wt_hash);
  WorkTree *remote_wt = ftwt(remote_wt_path);
  if(current_wt == NULL){
    err_logf(E_ERR, E_MSG_FUNC_NULL, "ftwt", remote_wt_hash);
  }

  List *conflicts = initList();

  WorkTree* merged_wt = mergeWorkTrees(current_wt, remote_wt, &conflicts);

  free(current_wt_path);
  free(remote_wt_path);
  
  free(current_commit_path);
  free(remote_commit_path);

  freeCommit(current_commit);
  freeCommit(remote_commit);

  freeWorkTree(current_wt);
  freeWorkTree(remote_wt);


  if(sizeList(conflicts) > 0){
    err_logf(E_OK, "La fusion de la branche %s et %s rencontre des conflits", current_branch, remote_branch);
    freeWorkTree(merged_wt);  
    free(current_wt_hash);
    free(current_branch_name);
    free(remote_wt_hash);
    free(current_branch);
    return conflicts;
  }
  freeList(conflicts);
  free(current_branch);

  char *hash = saveWorkTree(merged_wt, ".");  

  Commit *merged_commit = createCommit(hash);

  for(int i = 0; i < merged_wt->n; i++)
      myGitAdd(merged_wt->tab[i].name);

  freeWorkTree(merged_wt);
  
  commitSet(merged_commit, PREDECESSOR_KEY, current_wt_hash);
  commitSet(merged_commit, MERGED_MESSAGE_KEY, remote_wt_hash);

  free(current_wt_hash);
  free(remote_wt_hash);

  if(message != NULL){
    commitSet(merged_commit, MESSAGE_KEY, message);
  }
  
  const char* commit_hash = blobCommit(merged_commit);

  createUpdateRef(current_branch_name, commit_hash);
  createUpdateRef("HEAD", commit_hash);

  deleteRef(remote_branch);

  restoreCommit(commit_hash);

  free(hash);
  freeCommit(merged_commit);
  free((void *)commit_hash);
  free(current_branch_name);

  return NULL;
}