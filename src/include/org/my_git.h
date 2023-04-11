#ifndef MY_GIT
#define MY_GIT

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

#endif