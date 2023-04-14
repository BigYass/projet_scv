#include <string.h>
#include <stdbool.h>
#include <malloc.h>

#include "../include/org/my_git.h"
#include "../include/org/workfile.h"
#include "../include/org/refs.h"
#include "../include/org/commit.h"
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
