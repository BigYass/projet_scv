#include <stdio.h>
#include <malloc.h>
#include <sys/stat.h>

#include "../include/const.h"

#include "../include/util/hash.h"
#include "../include/util/liste.h"
#include "../include/util/file.h"
#include "../include/util/tool.h"


#include "../include/org/workfile.h"
#include "../include/org/branch.h"
#include "../include/org/commit.h"
#include "../include/org/my_git.h"
#include "../include/org/refs.h"


#include "../include/test/test.h"
#include "../include/test/debug.h"


void init_test(){
    init_tool();
    mkdir(TEST_DIRECTORY, 0777);
}

void test_hash(){
    printf("=== DEBUT DU TEST DES HASH ===\n");
  // TEST HACHAGE
    static const char filename[] = "Makefile";
    
    printf("Hash de %s : ", filename);
    char *hash = sha256file(filename);
    if(hash == NULL){
        printf(RED "sha256file(\"%s\") renvoie null...\n" RESET, filename);
        return;
    }
    printf("%s%s%s\n",GREEN, hash, RESET);

    printf("Conversion en chemin..");
    char *path = hashToPath(hash);
    if(path == NULL){
        printf(RED "hashToPath(\"%s\") renvoie null...\n" RESET, hash);
        return;
    }

    printf("Résultat : " YELLOW "%s" RESET "\n", path);

    const char text[] = "burger";

    printf("Hashage de \""YELLOW"%s"RESET"\" avec l'algorithme sdbm\n", text);
    printf("Résultat : " YELLOW "0x%lx" RESET "\n", sdbm((unsigned char*)text));


    printf("=== FIN DU TEST DES FONCTIONS DE HACHAGE ===\n");
    
    free(hash);
    free(path);
}

void test_list(){
    printf("=== DEBUT DU TEST DES LISTES ===\n");
    // TEST CELLULES ET LISTES
    puts("Creation de la liste et des cellules...");
    List* l = initList();
    
    insertFirst(l,buildCell("Amethyste"));
    insertFirst(l,buildCell("Diamant"));
    insertFirst(l,buildCell("Emeraude"));
    insertFirst(l,buildCell("Rubis"));
    insertFirst(l,buildCell("Saphir"));

    puts("Conversion de la liste en chaine de charactere...");
    char* s = ltos(l);
    printf("Conversion : %s%s%s\n",GREEN, s, RESET);

    puts("Reconversion de la chaine en liste...");
    List* l2 = stol(s);
    printf("Résultat : %s%s%s\n", GREEN, ltos(l2), RESET);

    printf("=== FIN DU TEST DES LISTES ===\n");
}

void test_files(){
    printf("=== DEBUT DU TEST DES FICHIERS ===\n");
    // TEST FICHIER ET DOSSIER
    
    static const char* filename = "Makefile";
    const char* hash = sha256file(filename);
    List *l2 = stol("Amethyste|Diamant|Emeraude|Rubis|Saphir");

    const char* tmpfile = TEST_DIRECTORY "/burger.list";
    printf("Stockage de la liste dans : %s%s%s \n", YELLOW, tmpfile, RESET);
    ltof(l2, tmpfile);

    printf("Récupération de la liste à partir du fichier %s%s%s...\n", YELLOW, tmpfile, RESET);
    List *l3 = ftol(tmpfile);
    printf("Résultat : %s%s%s\n", GREEN, ltos(l3), RESET);

    puts("Listage de tout les fichiers du dossier courant...\n");
    List* all_files = listdir(".");
    printf("Résultat : %s%s%s\n", GREEN, ltos(all_files), RESET);

    printf("Vérification de l'existence de : %s%s%s\n", YELLOW, filename, RESET);
    printf("Résultat : %s\n", TO_BOOL(file_exists(filename)));

    const char* cp_file = TEST_DIRECTORY "/Burger.txt";
    printf("Copiage de %s vers %s\n", filename, cp_file);
    cp(cp_file, filename);

    printf("Création du chemin du dossier de l'instantané de %s\n", filename);
    printf("Résultat : %s%s%s\n", GREEN, hashToPath(hash), RESET);

    printf("Création de l'instantatané de %s%s%s\n", GREEN, filename, RESET);
    blobFile(filename);

    printf("=== FIN DU TEST DES FICHIERS ===\n");
}

void test_work_file(){
    printf("=== DEBUT DU TEST DES WORKFILES ===\n");
    // TEST DES WORKFILE
    static const char* filename = "Makefile";
    printf("Autorisation de %s%s%s : %s%o%s\n", YELLOW, filename, RESET, RED, getChmod(filename), RESET);

    printf("Création d'un workfile...\n");
    WorkFile *wf = createWorkFile(filename);
    char *s = wfts(wf);
    printf("Conversion : %s%s%s\n\n", GREEN, s, RESET);

    printf("Convertion en Workfile...\n");
    WorkFile *wf_tmp = stwf(s);
    printf("Conversion : %s%s%s\n\n", GREEN, wfts(wf_tmp), RESET);

    printf("Cratioin d'un WorkTree...\n");
    WorkTree *wt = initWorkTree();

    printf("Ajout de WorkFiles dans le WorkTree...\n");
    appendWorkTree(wt, wf->name, wf->hash, wf->mode);
    appendWorkTree(wt, "__burger", "no", 0777);
    appendWorkTree(wt, "__cheese", "yes", 0777);
    appendWorkTree(wt, "__onions", "5f", 0777);
    

    printf("%s%s%s dans le WorkTree ? %s\n", YELLOW, wf->name, RESET, TO_BOOL(inWorkTree(wt, wf->name)));

    char *s2 = wtts(wt);
    printf("Conversion :\n %s\n", s2);

    printf("Conversion de la chaine en WorkTree...\n");

    WorkTree *wt_tmp = stwt(s2);

    printf("Résultat :\n%s\n", wtts(wt_tmp));

    // MANIPULATION DE FICHIERS
    const char worktree_file[] = TEST_DIRECTORY "/worktree.t";

    printf("Stockage du WorkTree dans le fichier "YELLOW"%s"RESET"..\n", worktree_file);
    wttf(wt, worktree_file);

    printf("Récupération du WorkTree dans " YELLOW "%s" RESET "..\n", worktree_file);
    WorkTree * wt_copy = ftwt(worktree_file);
    
    printf("Résultat : " YELLOW "%s" RESET "\n", wtts(wt_copy));

    printf("=== FIN DU TEST DES WORKFILES ===\n");
}

void test_branch(){
    printf("=== DEBUT DU TEST DES BRANCHES ===\n");
    printf("Initialisation des branches... (master)\n");
    initBranch();

    printf("Est ce que la branche "YELLOW"%s"RESET " existe ? %s\n", "master", TO_BOOL(branchExists("master")));
    printf("Est ce que la branche "YELLOW"%s"RESET " existe ? %s\n", "branch_invisible", TO_BOOL(branchExists("branch_invisible")));

    const char new_branch_name[] = "burger";

    printf("Est ce que la branche "YELLOW"%s"RESET " existe ? %s\n", new_branch_name, TO_BOOL(branchExists(new_branch_name)));
    
    printf("Création de la branche "YELLOW"%s"RESET"\n", new_branch_name);
    createBranch(new_branch_name);

    printf("Est ce que la branche "YELLOW"%s"RESET " existe ? %s\n", new_branch_name, TO_BOOL(branchExists(new_branch_name)));

    const char *current_branch_name = getCurrentBranch();
    printf("La branche courrante est "YELLOW "%s" RESET "\n", current_branch_name);

    printf("Affichage des commits de " YELLOW "%s" RESET " :\n", current_branch_name);
    printBranch(current_branch_name);

    printf("List de " YELLOW "%s" RESET ": \n", current_branch_name);
    printf("%s\n", ltos(branchList(current_branch_name)));

    printf("List de tout les commits :\n");
    printf("%s\n", ltos(getAllCommits()));

    printf("=== FIN DU TEST DES BRANCHES ===\n");
}

void test_commit(){
    printf("=== DEBUT DU TEST DES COMMITS ===\n");
    // Teste des pair (clé, valeur)
    printf("Création d'une paire (\"cle\", \"valeur\")");
    kvp* pair = createKeyVal("cle", "valeur");

    printf("Conversion de la paire en chaine\n");
    char *pair_string = kvts(pair);
    printf("Résultat \"" YELLOW "%s" RESET "\"\n", pair_string);

    printf("Conversion en clé à nouveau..\n");
    kvp* pair_copy = stkv(pair_string);
    printf("Résultat \"" YELLOW "%s" RESET "\"\n", kvts(pair_copy));

    // Teste des commits
    printf("Création d'un commit..\n");
    Commit *commit = initCommit();
    printf("Résultat (conversion) : \n%s\n", cts(commit));

    printf("Ajout de la paire " YELLOW "%s" RESET " dans le commit", pair_string);
    commitSet(commit, pair->key, pair->value);

    kvp *test_pair = createKeyVal("test", "valide"); 

    printf("Ajout de la paire " YELLOW "%s" RESET " dans le commit", kvts(test_pair));
    commitSet(commit, test_pair->key, test_pair->value);

    printf("Retrait de valeurs : \n");

    const char *test_keys[] = {
        pair->key,
        "test",
        "tree",
        "",
        NULL
    };
    
    for(size_t i = 0; i < sizeof_a(test_keys); i++){
        printf("commit["YELLOW"\"%s\""RESET"] = "GREEN"%s"RESET"\n", test_keys[i], commitGet(commit, test_keys[i]));
    }

    printf("Conversion du commit en chaine de charactère..\n");

    char *commit_string = cts(commit);
    printf("Résultat : "YELLOW"%s"RESET, commit_string);

    Commit *commit_copy = stc(commit_string);
    printf("Conversion inverse..\n");
    printf("Résultat : "YELLOW"%s"RESET, cts(commit_copy));

    const char file_name[] = TEST_DIRECTORY "/test_commit.commit";

    printf("Stockage du commit dans le ficheir " YELLOW "%s" RESET "..\n", file_name);
    ctf(commit, file_name);

    printf("Récupération du commit stocké dans " YELLOW "%s" RESET "..\n", file_name);
    Commit * commit_copy_2 = ftc(file_name);

    printf("Résultat : \n%s\n", cts(commit_copy_2));

    printf("Création d'un instantanné du commit orginal\n");
    blobCommit(commit);

    // TODO : free
    printf("=== FIN DU TEST DES COMMITS ===\n");
}

void test_my_git(){
    printf("=== DEBUT DU TEST DE MY_GIT ===\n");
    // On crée un fichier de test
    const char test_file_name[] = TEST_DIRECTORY "/test.txt";

    if(!file_exists(test_file_name)){
        printf("Création du fichier %s\n", test_file_name);
        FILE *file = fopen(test_file_name, "w");
        if(file != NULL){
            fputs("Fichier pour test. Burger", file);
            fclose(file);
        }
        else {
            err_logf(E_WARN, "Problème lors de l'écriture du fichier %s", test_file_name);
        }
    }

    printf("Ajout dans du fichier "YELLOW"%s"RESET" au projet..\n", test_file_name);
    myGitAdd(test_file_name);

    const char branch_test_name[] = "test";
    printf("Création de la branche " YELLOW "%s" RESET "..\n", branch_test_name);

    createBranch(branch_test_name);

    const char test_message[] = "Ceci un commit de test";

    printf("Commit de la branche " YELLOW "%s" RESET " avec le message \" " YELLOW "%s" RESET " \"..\n", branch_test_name, test_message);
    myGitCommit(branch_test_name, test_message);

    // TODO : free
    printf("=== FIN DU TEST DE MY_GIT ===\n");
}

void test_refs(){
    printf("=== DEBUT DU TEST DES REFERENCES ===\n");
    const char ref_name[] = "test_ref";

    int content_len = 7;
    const char *content = random_str(content_len);

    printf("Création ou mise à jour de la référence %s avec %s", ref_name, content);
    createUpdateRef(ref_name, content);

    printf("Contenu de la référence : %s", getRef(ref_name));

    printf("Supression de la référence %s..", ref_name);
    deleteRef(ref_name);

    // TODO : free
    printf("=== FIN DU TEST DES REFERENCES ===\n");
}