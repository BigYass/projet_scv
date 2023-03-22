#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "const.h"
#include "test.h"

#include "hash.h"

#include "file.h"
#include "liste.h"

#include "commit.h"
#include "refs.h"


void create_tmp_file(){
    mkdir(TMP_DIRECTORY, 0700);
}

int main()
{
    create_tmp_file();

    printf("%s === TEST DES FONCTIONS DE HACHAGE === \n%s", RED, RESET);
    test_hash();

    printf("\n%s === TEST DES FONCTIONS DE LISTE === \n%s", RED, RESET);
    test_list();

    printf("\n%s === TEST DES FONCTIONS DE MANIPULATION DE FICHIER === \n%s", RED, RESET);
    test_files();

    // FIN DU PROGRAMME
    puts("Fin du programme");
    return 0;
}


