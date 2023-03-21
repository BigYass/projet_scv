#ifndef LISTE_LIB
#define LISTE_LIB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <linux/limits.h>
#include <dirent.h>

typedef struct cell {
    char* data;
    struct cell* next;
} Cell;

typedef Cell* List;

//Gestion de liste

/**
 * @brief Initialise une liste vide de taille size_t
 * 
 * @return List* La liste vide aloué avec malloc
 */
List* initList();

/**
 * @brief Alloue la mémoire pour une cellule d'une liste dont data est égale à h 
 * 
 * @param h Chaine de cahractère représentant la cellule
 * @return Cell* La cellule en question
 */
Cell* buildCell(const char* h);

/**
 * @brief Insère une cellule préalablement aloué pour l'ajouter en en-tête
 * 
 * @param L La liste dans laquelle sera ajouté la cellule
 * @param C La cellule à ajouter
 */
void insertFirst(List *L, Cell* C);

/**
 * @brief Libère la mémoire de la liste. Libère également les cellules
 * 
 * @param L Liste à libérér
 */
void freeList(List *L);

/**
 * @brief Convertit une cellule en chaine de charactère (renvoie c->data)
 * 
 * @param c La cellule à convertir
 * @return char* Le contenu de c->data. De la mémoire est alloué.
 */
char* ctos(Cell* c);

/**
 * @brief Convertie une liste en chaine de charactère dans le format : "cellule1|cellule2|[...]|celluleN"
 * 
 * @param L La liste à convertir
 * @return char* Chaine de charactère nouvellement alloué de la liste
 */
char* ltos(List* L);

/**
 * @brief Renvoie le i-ème éléménts de la liste. NULL si il n'est pas dans la liste
 * 
 * @param L La liste
 * @param i Nombre
 * @return Cell* Cellule étant le i-ème élément de la liste.
 */
Cell* listGet(List* L, int i);

/**
 * @brief Recherche une cellule ayant pour data str
 * 
 * @param L La liste qui sera fouillé
 * @param str La chaine de charactère à comparer
 * @return Cell* La cellule ayant pour data str. NULL si non trouvé
 */
Cell* searchList(List* L, const char* str);

/**
 * @brief Convertit une chaine de charactère en Liste en séparant les '|'.
 * 
 * @param s La chaine de charactère représentatif de la liste
 * @return List* La liste récupérer de la chaine de charactère
 */
List* stol(const char* s);

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
 * @brief Crée le chemin du dossier de fichier pour crée son instatané
 * 
 * @param hash hash du fichier
 * @return char* Chemin du dossier
 */
char* hashToPath(const char* hash);

/**
 * @brief Fait un instantané du fichier
 * 
 * @param file Nom du fichier à enregistrer
 */
void blobFile(const char* file); 


#endif