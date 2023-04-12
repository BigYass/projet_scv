#ifndef LISTE_LIB
#define LISTE_LIB

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
 * @brief Libère la mémoire d'une cellule
 * 
 * @param c Cellule à libéré
 */
void freeCell(Cell *c);

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
 * @brief Filtre une liste pour les éléments commencant par paterne.
 * 
 * @param L La liste à filtré
 * @param pattern Le paterne
 * @return List* La liste filtré
 */
List *filterList(List *L, const char* pattern);

/**
 * @brief Retourne la taille d'une liste
 * 
 * @param L Liste
 * @return int Taille de la liste
 */
int sizeList(List *L);

#endif