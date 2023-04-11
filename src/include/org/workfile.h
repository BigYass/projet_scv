#ifndef WORKFILE
#define WORKFILE

#define WORKTREE_SIZE 0x100

#include "../util/liste.h"

typedef struct {
  char *name;
  char *hash;
  int mode;
} WorkFile;

typedef struct {
  WorkFile *tab;
  int size;
  int n;
} WorkTree;

// === PERMISSIONS ===

/**
 * @brief Renvoie les autorisations d'un fichier
 * 
 * @param path Chemin vers le fichier
 * @return int Les autorisations 
 */
int getChmod(const char *path);

/**
 * @brief Change les autorisations d'un fichier au chemin path avec la commande chmod <mode> <fichier>
 * 
 * @param mode Nouvelle autorisation
 * @param path Chemin du fichier
 */
void setMode (int mode , char * path);

// === MANIPULATION DE WORKFILE

/**
 * @brief Initialise un WorkFile avec son nom.
 * 
 * @param name Nom du fichier
 * @return * WorkFile* Le workfile en question
 */
WorkFile* createWorkFile(const char* name);

/**
 * @brief Libère un workfile
 * 
 * @param wf Le workfile à libéré
 */
void freeWorkFile(WorkFile *wf);

/**
 * @brief Libère un worktree
 * 
 * @param wt Le worktree à libéré
 */
void freeWorkTree(WorkTree *wt);
/**
 * @brief Convertit un WorkFile en chaine de charactère
 * 
 * @param wf Workfile
 * @return char* Chaine de charactère représentant le workfile
 */
char* wfts(WorkFile* wf);

/**
 * @brief Convertit une chaine de charactère en WorkFile
 * 
 * @param ch La chaine de charactère
 * @return WorkFile* Le WorkFile initialisé
 */
WorkFile* stwf(const char* ch);

/**
 * @brief Initialise un workfile 
 * 
 * @return WorkTree* Le workfile initialisé
 */
WorkTree* initWorkTree();

/**
 * @brief Vérifie la présence d'un fichier ou d'un répertoire du nom de `name` dans le worktree `wt`
 * 
 * @param wt Le worktree où l'on cherche le fichier oue le répertoire
 * @param name Nom du fichier ou du répertoire
 * @return int Retourne la position du fichier. -1 si non trouvé...
 */
int inWorkTree(WorkTree* wt, const char* name);

/**
 * @brief Rajoute un fichier au WorkTree SEULEMENT si il n'existe pas
 * 
 * @param wt Le worktree où l'on rajoute le fichier ou le répertoire
 * @param name Nom du fichier ou du répertoire
 * @param hash Hash du fichier ou du répertoire
 * @param mode Permission du fichier
 * @return int Renvoie le résultat de la fonction
 */
int appendWorkTree(WorkTree* wt, const char* name, const char* hash, int mode);
 
/**
 * @brief Convertit un WorkTree en une chaine de charactère composée des représentation 
 * du WorkTree séparées par de saut de ligne ('\\n')
 * 
 * @param wt Le WorkTree à convertir
 * @return char* La chaine de charactère représentant le WorkTree
 */
char* wtts(WorkTree* wt);

/**
 * @brief Convertit une chaine de charactère en un worktree
 * 
 * @param s La chaine de charactère
 * @return WorkTree* Le worktree résultant de la conversion. NULL si la conversion a échoué
 */
WorkTree* stwt(char *s);

// === MANIPULATION DE FICHIER ===

/**
 * @brief Ecrit dans le fichier 'file' le chaine de charactère représentés par 'wt'
 * 
 * @param wt Le WorkTree à conservé dans un fichier
 * @param file Le chemin du fichier où stocké le WorkTree
 * @return int Résultat de la fonciton
 */
int wttf(WorkTree* wt, const char* file);

/**
 * @brief Lit le fichier 'file' et tente de le convertir en WorkTree
 * 
 * @param file Chemin du fichier à lire
 * @return WorkTree* Le WorkTree correspondant si la conversion a réussie. NULL sinon...
 */
WorkTree* ftwt(const char* file);

/**
 * @brief Convertit le hash d'un worktree en son chemin d'accès
 * 
 * @param hash Le hash du commit
 * @return char* Le chemin d'accès au worktree
 */
char* workTreePath(const char* hash);

/**
 * @brief Fait un instantanné d'un WorkTree (le fichier ou répertoire) et renvoie son hash
 * 
 * @param wt Le WorkTree
 * @return char* Hash du fichier temporaire.
 */
char* blobWorkTree(WorkTree* wt);

/**
 * @brief Crée un enregistrement instantané du contenu du WorkTree
 * 
 * @param wt Le WorkTree à enregistrer
 * @param path Chemin où enregistrer le fichier
 * @return char* Renvoie le hash 
 */
char* saveWorkTree(WorkTree* wt, char* path);

/**
 * @brief Réstaure un fichier de sauvegarde représentant un répertoire de travail
 * 
 * @param wt Le Worktree utilisé
 * @param path Le chemin du fichier
 */
void restoreWorkTree(WorkTree* wt, char* path);

/**
 * @brief Fusionne de WorkTrees dans un troisième
 * 
 * @param wt1 Premier WorkTree
 * @param wt2 Deuxième WorkTree
 * @param conflicts Une liste contenant les éléments à conflit
 * @return WorkTree* La fusion
 */
WorkTree *mergeWorkTrees(WorkTree *wt1, WorkTree *wt2, List **conflicts);

#endif