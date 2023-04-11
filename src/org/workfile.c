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




// TODO : O bordel

int getChmod(const char *path)
{
  struct stat ret;
  if (stat(path, &ret) == -1)
  {
    return -1;
  }
  return (ret.st_mode & S_IRUSR) | (ret.st_mode & S_IWUSR) | (ret.st_mode & S_IXUSR) | /*owner*/
         (ret.st_mode & S_IRGRP) | (ret.st_mode & S_IWGRP) | (ret.st_mode & S_IXGRP) | /*group*/
         (ret.st_mode & S_IROTH) | (ret.st_mode & S_IWOTH) | (ret.st_mode & S_IXOTH);  /*other*/
}

void setMode(int mode, char *path)
{
  char cmd[MAX_BUF_SIZE] = "\0";

  sprintf(cmd, "chmod %d \"%s\"", mode, path);

  system(cmd);
}

WorkFile *createWorkFile(const char *name)
{
  WorkFile *wf = malloc(sizeof(WorkFile));

  memset(wf, 0, sizeof(WorkFile));

  wf->name = strdup(name);
  wf->hash = NULL;
  wf->mode = 0;

  return wf;
}

void freeWorkFile(WorkFile *wf)
{
  if(wf != NULL){
  if(wf->hash != NULL) free(wf->hash);
  if(wf->name != NULL) free(wf->name);
  free(wf);
  }
}

char *wfts(WorkFile *wf)
{
  char *s = malloc(sizeof(char) * MAX_BUF_SIZE);

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
  wt->tab = malloc(WORKTREE_SIZE * sizeof(WorkFile));
  memset(wt->tab, 0, WORKTREE_SIZE * sizeof(WorkFile));
  wt->size = WORKTREE_SIZE;
  wt->n = 0;

  return wt;
}

void freeWorkTree(WorkTree *wt){
  if(wt->tab != NULL)
    freeWorkFile(wt->tab);
  if(wt != NULL)
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
    err_log(E_WARN, "name NULL ou vide");
    return -3;
  }
  if (inWorkTree(wt, name) >= 0)
  {
    err_logf(E_WARN, "%s est déjà existant, ajout impossible", name);
    return -1;
  }
  else if (wt->size <= wt->n)
  {
    err_log(E_WARN, "Ajout impossible, le WortTree est plein");
    return -2;
  }
  wt->tab[wt->n].mode = mode;
  wt->tab[wt->n].name = strdup(name);
  wt->tab[wt->n++].hash = hash ? strdup(hash) : NULL;

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

    if (appendWorkTree(wt, wf->name, wf->hash, wf->mode))
    {
      free(wf);
      break;
    }
    free(wf);

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
    err_logf(E_ERR, "Erreur lors de l'ouverture du ficher %s", file);
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
    err_logf(E_ERR, "Erreur lors de la lecture du fichier %s", file);
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

  WorkTree *wt = stwt(s);
  free(s);
  return wt;
}

char* workTreePath(const char* hash){
  if(hash == NULL){
    err_log(E_WARN, "Tentative de conversion avec un hash null");
    return NULL;
  }

  char *hash_path = hashToPath(hash);

  if(hash_path == NULL){
    err_logf(E_WARN, RED "hashToPath(\"%s\") " RESET " a renvoyé NULL", hash);
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
    strcat(full_path, "/");
    strcat(full_path, wt->tab[i].name);

    if (file_exists(full_path))
    {
      blobFile(full_path);
      wt->tab[i].hash = sha256file(full_path);
      wt->tab[i].mode = getChmod(full_path);
    }
    else
    {
      WorkTree *wt_rep = initWorkTree();
      List *L = listdir(full_path);
      for (Cell *cursor = *L; cursor; cursor = cursor->next)
      {
        if (ctos(cursor)[0] == '.')
          continue;
        appendWorkTree(wt_rep, ctos(cursor), NULL, 0);
      }

      wt->tab[i].hash = saveWorkTree(wt_rep, full_path);
      wt->tab[i].mode = getChmod(full_path);
    }
  }
  return blobWorkTree(wt);
}

int isWorkTree(char *hash)
{
  if (file_exists(strcat(filePath(hash), ".t")))
  {
    return 1;
  }
  if (file_exists(filePath(hash)))
  {
    return 0;
  }
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
      setMode(getChmod(copyPath), full_path);
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
      if(!strcmp(wt1->tab[i].name, wt2->tab[i].name))
        insertFirst(*conflicts, buildCell(wt1->tab[i].name));
  

  // On rajoute les éléments de wt1
  if(wt1)
    for(int i = 0; i < wt1->n; i++)
      if(searchList(*conflicts, wt1->tab[i].name))
        appendWorkTree(wt, wt1->tab[i].name, wt1->tab[i].hash, wt1->tab[i].mode);

  // On rajoute les éléments de wt2
  if(wt2)
    for(int i = 0; i < wt2->n; i++)
      if(searchList(*conflicts, wt2->tab[i].name))
        appendWorkTree(wt, wt2->tab[i].name, wt2->tab[i].hash, wt2->tab[i].mode);

  return wt;
}

List* merge(const char* remote_branch, const char* message){
  char *current_branch = getRef(getCurrentBranch());

  char *current_commit_path = commitPath(current_branch);
  if(current_commit_path == NULL){
    err_logf(E_ERR, "Impossible de trouver le chemin du fichier correspondant à la branche %s..", current_branch);
  }
  
  char *remote_commit_path = commitPath(remote_branch);
  if(current_commit_path == NULL){
    err_logf(E_ERR, "Impossible de trouver le chemin du fichier correspondant à la branche %s..", remote_branch);
  }

  Commit *current_commit = ftc(current_commit_path);
  if(current_commit == NULL){
    err_logf(E_ERR, "Conversion vers commit du fichier %s impossible..", current_commit_path);
  }

  Commit *remote_commit = ftc(remote_commit_path);
  if(current_commit == NULL){
    err_logf(E_ERR, "Conversion vers commit du fichier %s impossible..", remote_commit_path);
  }

  char *current_wt_hash = commitGet(current_commit, "tree");
  if(current_wt_hash == NULL){
    err_logf(E_ERR, "Impossible de trouver tree dans le commit %s", current_commit_path);
  }

  char *remote_wt_hash = commitGet(remote_commit, "tree");
  if(remote_wt_hash == NULL){
    err_logf(E_ERR, "Impossible de trouver tree dans le commit %s", remote_commit_path);
  }

  WorkTree *current_wt = ftwt(workTreePath(current_wt_hash));
  if(current_wt == NULL){
    err_logf(E_ERR, "Conversion du worktree %s", current_wt);
  }

  WorkTree *remote_wt = ftwt(workTreePath(remote_wt_hash));
  if(current_wt == NULL){
    err_logf(E_ERR, "Conversion du worktree %s", current_wt);
  }

  List *conflicts = initList();

  WorkTree* merged_wt = mergeWorkTrees(current_wt, remote_wt, &conflicts);

  if(sizeList(conflicts) > 0){
    err_logf(E_OK, "La fusion de la branche %s et %s rencontre des conflits", current_branch, remote_branch);
    return conflicts;
  }

  char *hash = saveWorkTree(merged_wt, ".");  

  Commit *merged_commit = createCommit(hash);
  
  commitSet(merged_commit, "predecessor", current_wt_hash);
  commitSet(merged_commit, "merged_predecessor", remote_wt_hash);

  if(message != NULL){
    commitSet(merged_commit, "message", remote_wt_hash);
  }
  
  const char* commit_hash = blobCommit(merged_commit);

  createUpdateRef(current_branch, commit_hash);
  createUpdateRef("HEAD", commit_hash);

  deleteRef(remote_branch);

  restoreCommit(commit_hash);

  free(current_branch);
  free(current_commit_path);
  free(remote_commit_path);
  freeCommit(current_commit);
  freeCommit(remote_commit);
  free(current_wt_hash);
  free(remote_wt_hash);
  freeWorkTree(current_wt);
  freeWorkTree(remote_wt);
  freeList(conflicts);
  freeWorkTree(merged_wt);
  free(hash);
  freeCommit(merged_commit);
  free((void *)commit_hash);

  return NULL;
}

void createDeletionCommit(const char *branch, List *conflicts, const char* message){
  char *current_branch = getCurrentBranch();
  myGitCheckoutBranch(branch);

  WorkTree *wt = ftwt(commitGet(ftc(commitPath(getRef(branch))), "tree"));

  remove(".add");

  for(int i = 0; i < wt->n; i++)
    if(searchList(conflicts, wt->tab[i].name))
      myGitAdd(wt->tab[i].name);

  myGitCommit(branch, message);

  myGitCheckoutBranch(current_branch);

  free(current_branch);
  freeWorkTree(wt);
}




