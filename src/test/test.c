#include <stdio.h>

#include "../const.h"
#include "../util/hash.h"
#include "../util/liste.h"
#include "../util/file.h"
#include "../org/workfile.h"
#include "test.h"

void test_hash(){
  // TEST HACHAGE
    static const char* filename = "Makefile";
    
    printf("Hash de %s : ", filename);
    char *hash = sha256file(filename);
    printf("%s%s%s\n",GREEN, hash, RESET);


    printf("=== FIN DU TEST DES FONCTIONS DE HACHAGE ===\n");
}

void test_list(){
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
    // TEST FICHIER ET DOSSIER
    
    static const char* filename = "Makefile";
    const char* hash = sha256file(filename);
    List *l2 = stol("Amethyste|Diamant|Emeraude|Rubis|Saphir");

    const char* tmpfile = ".tmp/burger.list";
    printf("Stockage de la liste dans : %s%s%s \n", YELLOW, tmpfile, RESET);
    ltof(l2, tmpfile);

    printf("Récupération de la liste à partir du fichier %s%s%s...\n", YELLOW, tmpfile, RESET);
    List *l3 = ftol(tmpfile);
    printf("Résultat : %s%s%s\n", GREEN, ltos(l3), RESET);

    puts("Listage de tout les fichiers du dossier courant...\n");
    List* all_files = listdir(".");
    printf("Résultat : %s%s%s\n", GREEN, ltos(all_files), RESET);

    printf("Vérification de l'existence de : %s%s%s\n", YELLOW, filename, RESET);
    printf("Résultat : %s%s%s\n", RED, TO_BOOL(file_exists(filename)), RESET);

    const char* cp_file = ".tmp/Burger.txt";
    printf("Copiage de %s vers %s\n", filename, cp_file);
    cp(cp_file, filename);

    printf("Création du chemin du dossier de l'instantané de %s\n", filename);
    printf("Résultat : %s%s%s\n", GREEN, hashToPath(hash), RESET);

    printf("Création de l'instantatané de %s%s%s\n", GREEN, filename, RESET);
    blobFile(filename);

    printf("=== FIN DU TEST DES FICHIERS ===\n");
}

void test_work_file(){
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
    

    printf("%s%s%s dans le WorkTree ? %s%s%s\n", YELLOW, wf->name, RESET, GREEN, TO_BOOL(inWorkTree(wt, wf->name)), RESET);

    char *s2 = wtts(wt);
    printf("Conversion :\n %s\n", s2);

    printf("Conversion de la chaine en WorkTree...\n");

    WorkTree *wt_tmp = stwt(s2);

    printf("Résultat :\n%s\n", wtts(wt_tmp));

    printf("=== FIN DU TEST DES WORKFILE ===\n");
}
