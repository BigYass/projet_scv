#include <stdio.h>
#include <stdlib.h>

#include "const.h"
#include "hash.h"
#include "liste.h"

#define TO_BOOL(NUM) (NUM ? "false" : "true")

int main()
{

    // TEST HACHAGE
    const char* filename = "Makefile";
    
    printf("Hash de %s : ", filename);
    char *hash = sha256file(filename);
    printf("%s%s%s\n\n",GREEN, hash, RESET);


    // TEST CELLULES ET LISTES
    puts("Creation de la liste et des cellules...\n");
    List* l = initList();
    
    insertFirst(l,buildCell("burger"));
    insertFirst(l,buildCell("cheese"));
    insertFirst(l,buildCell("bacon"));
    insertFirst(l,buildCell("couscous"));
    insertFirst(l,buildCell("tagine"));

    puts("Conversion de la liste en chaine de character...\n");
    char* s = ltos(l);
    printf("Conversion : %s%s%s\n",GREEN, s, RESET);

    puts("Reconversion de la chaine en liste...\n");
    List* l2 = stol(s);
    printf("Résultat : %s%s%s\n\n", GREEN, list_string(l2), RESET);


    // TEST FICHIER ET DOSSIER
    const char* tmpfile = ".tmp/burger.list";
    printf("Stockage de la liste dans : %s%s%s \n", YELLOW, tmpfile, RESET);
    ltof(l2, tmpfile);

    printf("Récupération de la liste à partir du fichier %s%s%s...\n", YELLOW, tmpfile, RESET);
    List *l3 = ftol(tmpfile);
    printf("Résultat : %s\n", ltos(l3));

    puts("Listage de tout les fichiers du dossier courant...\n");
    List* all_files = listdir(".");
    printf("Résultat : %s\n", list_string(all_files));
    
    printf("Vérification de l'existence de : %s%s%s\n", YELLOW, filename, RESET);
    printf("Résultat : %s%s%s\n", RED, TO_BOOL(file_exists(filename)), RESET);

    const char* cp_file = "Burger.txt";
    printf("Copiage de %s vers %s\n", filename, cp_file);
    cp(cp_file, filename);

    printf("Création du chemin du dossier de l'instantané de %s\n", filename);
    printf("Résultat : %s%s%s\n", GREEN, hashToPath(hash), RESET);

    printf("Création de l'instantatané de %s%s%s\n", GREEN, filename, RESET);
    blobFile(filename);

    // FIN DU PROGRAMME
    puts("Fin du programme");
    return 0;
}


