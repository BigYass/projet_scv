#ifndef MY_GIT
#define MY_GIT

#include "../util/liste.h"

/**
 * @brief Ajoute un fichier dans le git
 * 
 * @param file Le nom du fichier
 */
void myGitAdd(const char* file);

/**
 * @brief Commit similaire à github
 * 
 * @param branch_name Nom de la branche
 * @param message Message du commit
 */
void myGitCommit(const char *branch_name, const char *message);

/**
 * @brief Change de branche
 * 
 * @param branch 
 */
void myGitCheckoutBranch(const char* branch);


/**
 * @brief Change de branche avec un pattern
 * 
 * @param pattern Le paterne
 */
void myGitCheckoutCommit(const char* pattern);

/**
 * @brief Fusionne la branche courrante avec la branche passé en paramètre si aucun conflit existe
 * 
 * @param remote_branch La branche à fusionné
 * @param message Le message du merge
 * @return List* La liste avec les hash en non-conflits
 */
List* merge(const char* remote_branch, const char* message);

#endif