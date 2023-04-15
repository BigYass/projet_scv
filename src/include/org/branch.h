#ifndef BRANCH
#define BRANCH

#include "../util/liste.h"

/**
 * @brief Initialise la branche master
 * 
 */
void initBranch();

/**
 * @brief Vérifie l'existence d'une branche
 * 
 * @param branch Nom de la branche
 * @return int Retourne 1 si elle existe, 0 sinon
 */
int branchExists(const char* branch);

/**
 * @brief Crée une référence appelée branch qui pointe vers le même commit que la référence HEAD
 * 
 * @param branch Le nom de la branche
 */
void createBranch(const char *branch);

/**
 * @brief Retourne le nom contenue dans .current_branch
 * 
 * @return char* Nom de la branche courrante
 */
char *getCurrentBranch();

/**
 * @brief Affiche le hash de tout les commit d'une branche
 * 
 * @param branch Nom de la branche
 */
void printBranch(const char *branch);

/**
 * @brief Construit et retroune la liste contenant le hash de tout les commit d'une branche
 * 
 * @param branch 
 * @return List* 
 */
List *branchList(const char *branch);

#endif