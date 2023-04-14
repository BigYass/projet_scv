#ifndef REFS
#define REFS

#define REFS_DIRECTORY ".refs"

/**
 * @brief Crée le répertoire caché REFS_DIRECTORY et les composants master et HEAD
 * 
 */
void initRefs();

/**
 * @brief Mets à jour ou crée un fichier de référence.
 * 
 * @param ref_name 
 * @param hash 
 */
void createUpdateRef(const char* ref_name, const char* hash);

/**
 * @brief Supprime un fichier de référence
 * 
 * @param ref_name le nom de la référence
 */
void deleteRef(const char* ref_name);

/**
 * @brief Récupère vers quoi pointe une référence 
 * 
 * @param ref_name 
 * @return char* 
 */
char* getRef(const char* ref_name);



#endif