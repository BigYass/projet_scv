#ifndef FILE_LIB
#define FILE_LIB

#include <stdbool.h>

#include "liste.h"

/**
 * @brief Crée un fichier
 * 
 * @param file Nom du fichier
 */
void createFile(const char* file);


/**
 * @brief Stocke la liste en format "cell1|cell2|...|cellN" dans le fichier à la location path
 * 
 * @param L La liste à stocker
 * @param path Le chemin du fichier
 */
void ltof(List* L, const char* path);

/**
 * @brief Lit le fichier et tente de le convertir en liste
 * 
 * @param path Chemin du fichier à lire. Si il n'existe pas NULL est renvoyé.
 * @return List* La liste réprésenté dans le fichier. NULL si erreur.
 */
List* ftol(const char* path);

//Gestion de fichier

/**
 * @brief Renvoie la liste des différents nom de fichier dans le dossier root_dir
 * 
 * @param root_dir Chemin du dossier 
 * @return List* Liste représentant les fichiers du dossier.
 */
List* listdir(const char* root_dir);

/**
 * @brief Vérifie si un fichier existe
 * 
 * @param file Le nom du fichier à vérifier
 * @return Retourne true si le fichier existe, false sinon.
 */
bool file_exists(const char *file);

/**
 * @brief Copie le fichier from vers le fichier to ligne par ligne
 * 
 * @param to Nom du fichier destinataire
 * @param from Nom du fichier source, si il n'existe pas rien ne se passe
 */
void cp(const char *to, const char *from);



/**
 * @brief Fait un instantané du fichier
 * 
 * @param file Nom du fichier à enregistrer
 */
void blobFile(const char* file); 



#endif