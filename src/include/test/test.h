#ifndef TEST
#define TEST

#define TEST_DIRECTORY ".test"

/**
 * @brief Initialise les choses nésséssaire pour les tests
 * 
 */
void init_test();

/**
 * @brief Teste les fonctions de hachage (util/hash.h)
 * 
 */
void test_hash();

/**
 * @brief Teste le bon fonctionnement des listes (util/liste.h)
 * 
 */
void test_list();

/**
 * @brief Teste le bon foncitonnement des fonction de traitement de fichier (util/file.h)
 * 
 */
void test_files();

/**
 * @brief Teste le bon foncitonnement des workfiles (org/workfile.h)
 * 
 */
void test_work_file();

/**
 * @brief Teste le bon foncitonnement des branches (org/branch.h)
 * 
 */
void test_branch();

/**
 * @brief Test le bon foncitonnement des commit (org/commit.h)
 * 
 */
void test_commit();

/**
 * @brief Test le bon foncitonnement de myGit (org/my_git.h)
 * 
 */
void test_my_git();

/**
 * @brief Test le bon foncitonnement des références (org/refs.h)
 * 
 */
void test_refs();

#endif