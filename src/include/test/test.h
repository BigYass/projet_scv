#ifndef TEST
#define TEST

#define TO_BOOL(NUM) ((NUM) ? "false" : "true")

/**
 * @brief Teste les fonctions de hachage (hash.h)
 * 
 */
void test_hash();

/**
 * @brief Teste le bon fonctionnement des listes (liste.h)
 * 
 */
void test_list();

/**
 * @brief Teste le bon foncitonnement des fonction de traitement de fichier (file.h)
 * 
 */
void test_files();

/**
 * @brief Teste le bon foncitonnement des workfiles (workfile.h)
 * 
 */
void test_work_file();

#endif