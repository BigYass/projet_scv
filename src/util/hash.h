#ifndef HASH_LIB
#define HASH_LIB

/**
 * @brief Fait la somme de hashage de l'algorithme sha256 et stock le résultat dans dans un fichier
 * 
 * @param source Nom du fichier source qui sera haché
 * @param dest Nom du fichier de sortie où le résultat sera stocké. Si le fichier n'existe pas, le fichier sera crée
 * @return int Retourne le résultat de l'appel de la fonction 
 * 
 */
int hashFile(const char *source, const char *dest);

/**
 * @brief Renvoie la somme de hachage de l'algorithme sha256 du fichier donné en paramètre.
 * 
 * @param file Nom du fichier. 
 * @return char* Somme de hachage. NULL en cas d'erreur
 */
char *sha256file(const char *file);

/**
 * @brief Crée le chemin du dossier de fichier pour crée son instatané
 * 
 * @param hash hash du fichier
 * @return char* Chemin du dossier
 */
char* hashToPath(const char* hash);

/**
 * @brief Fonction de hachage d'une chaine de charactère utilisant l'algoithme de sdbm
 * 
 * @param str La chaine de charactère
 * @return unsigned long Le hash
 */
static unsigned long
sdbm(unsigned char *str);

#endif