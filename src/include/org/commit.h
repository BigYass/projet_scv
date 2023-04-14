#ifndef COMMIT
#define COMMIT

#define COMMIT_SIZE 0x100

#define PREDECESSOR_KEY "predecessor"
#define MESSAGE_KEY "message"
#define MERGED_MESSAGE_KEY "merged_predecessor"
#define TREE_KEY "tree"

typedef struct key_value_pair {
  char* key;
  char* value;
} kvp;

typedef struct hash_table {
  kvp** T;
  int n;
  int size;
} HashTable;

typedef HashTable Commit;

/**
 * @brief Alloue et initialise un élément Key Value Pair
 * 
 * @param key Clé de l'élément
 * @param val Valeur de l'élement
 * @return kvp* La paire alloué
 */
kvp* createKeyVal(const char* key, const char* val);

/**
 * @brief Libère une paire de clé et d'une valeur
 * 
 * @param kv La paire à libérer
 */
void freeKeyVal(kvp* kv);

/**
 * @brief Libère le commit
 * 
 * @param c Le commit à libéré
 */
void freeCommit(Commit *c);


/**
 * @brief Convertit un élément en chaine de charactère de la forme "clé : valeur"
 * 
 * @param k Clé à convertit
 * @return char* La chaine de charactère formaté
 */
char* kvts(kvp* k);

/**
 * @brief Convertit une chaine de charactère en paire qui est alloué
 * 
 * @param str La chaine à convertir
 * @return kvp* La paire nouvellement alloué
 */
kvp* stkv(const char* str);

/**
 * @brief Alloue et initialise un Commit de taille fix
 * 
 * @return Commit* Le commit nouvellement alloué
 */
Commit* initCommit();

/**
 * @brief Insère une paire (key, value) dans la table.
 * 
 * @param c Commit
 * @param key Clé
 * @param value Valeur
 */
void commitSet(Commit* c, const char* key, const char* value);

/**
 * @brief Alloue et initialise un Commit puis y a ajoute l'élément obligatoire de clé TREE_KEY
 * 
 * @param hash Valeur de TREE_KEY
 * @return Commit* Commit nouvellement alloué
 */
Commit* createCommit(const char* hash);

/**
 * @brief Cherche dans la table un élément dont la clé est key
 * 
 * @param c Le commit
 * @param key La clé à recherché
 * @return char* Valeur de la clé si trouvé, NULL sinon
 */
char* commitGet(Commit* c, const char* key);

/**
 * @brief Convertit le hash d'un commit en son chemin d'accès
 * 
 * @param hash Le hash du commit
 * @return char* Le chemin d'accès au commit
 */
char* commitPath(const char* hash);

/**
 * @brief Convertit un commit en une chaine de charactère
 * 
 * @param c Le commit
 * @return char* Le résultat
 */
char* cts(Commit* c);

/**
 * @brief Convertit une chaine de charectère en Commit
 * 
 * @param s La chaine de charactère
 * @return Commit* Résultat
 */
Commit* stc(char* s);

/**
 * @brief Ecrit un commit dans un fichier
 * 
 * @param c commit
 * @param file nom du fichier
 */
void ctf(Commit* c, const char* file);

/**
 * @brief Lit un fichier pour y récupérer un commit
 * 
 * @param file Le nom du fichier
 * @return Commit* Résultat
 */
Commit* ftc(const char* file);

/**
 * @brief Crée un instantanné d'un commit
 * 
 * @param c Le commit
 * @return char* hash du fichier représentant le commit
 */
char* blobCommit(Commit* c);

/**
 * @brief Réstaure le worktree associé à un commit 
 * 
 * @param hash Hash du commit
 */
void restoreCommit(const char *hash);

/**
 * @brief Change de branche avec un pattern
 * 
 * @param pattern Le paterne
 */
void myGitCheckoutCommit(const char* pattern);

#endif