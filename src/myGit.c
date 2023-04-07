#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "include/const.h"

#include "include/test/test.h"

#include "include/org/refs.h"
#include "include/org/my_git.h"
#include "include/org/workfile.h"

#include "include/util/file.h"


void create_tmp_file(){
    mkdir(TMP_DIRECTORY, 0700);
}

int main(int argc, char *argv[])
{
    if(argc <= 0){
        printf("Comment ?\n");
        return 0;
    }

    if(argc == 1){
        printf("Utilisation : \n");
        printf(YELLOW "%s init :"RESET" initialise le repertoire de references.\n", argv[0]);
        printf(YELLOW "%s list-refs :"RESET" affiche toutes les references existantes.\n", argv[0]);
        printf(YELLOW "%s create-ref <name> <hash> :"RESET" cree la reference <name> qui pointe vers le commit correspondant au hash donne.\n", argv[0]);
        printf(YELLOW "%s delete-ref <name> :"RESET" supprime la reference name.\n", argv[0]);
        printf(YELLOW "%s add <elem> [<elem2> <elem3> ...] :"RESET" ajoute un ou plusieurs fichiers/repertoires `a la zone de preparation (pour faire partie du prochain commit).\n", argv[0]);
        printf(YELLOW "%s list-add :"RESET" affiche le contenu de la zone de preparation.\n", argv[0]);
        printf(YELLOW "%s clear-add :"RESET" vide la zone de preparation.\n", argv[0]);
        printf(YELLOW "%s commit <branch name> [-m <message>] :"RESET" effectue un commit sur une branche, avec ou sans message descriptif.\n", argv[0]);
        printf(YELLOW "%s test :"RESET" Lance des tests sur les fonctionnalites.\n", argv[0]);
    }


    create_tmp_file();

    if (!strcmp(argv[1], "init")){
        initRefs();
        printf("Projet initialisé!");
    }
    else if(!strcmp(argv[1], "refs-list")){
        if(!file_exists(".refs")){
            printf("Le fichier .refs n'existe. Veuiller initialiser le programme avec la commande %s init\n", argv[0]);
            return 0;
        }
        printf("refs :\n");

        List *L = listdir(".refs");
        for(Cell* cursor = *L; cursor; cursor = cursor->next){
            if (cursor->data[0] == '.') continue;
            char *content = getRef(cursor->data);
            printf("- %s \t %s\n", cursor->data, content);
            free(content); //A supprime pour performance ? Le programme s'arrete assez rapidement de toute facon
        }
    }
    else if(!strcmp(argv[1], "create-ref")){
        if(argc < 4){
            printf("Utilisation : %s create-ref <name> <hash>\n", argv[0]);
            return 0;
        }
        createUpdateRef(argv[2], argv[3]);
        printf("La branche %s a ete mis a jour avec le hash %s\n", argv[2], argv[3]);
    }
    else if(!strcmp(argv[1], "delete-ref")){
        if(argc < 3){
            printf("Utilisation : %s delete-ref <name>\n", argv[0]);
            return 0;
        }
        deleteRef(argv[2]);
        printf("La reference " YELLOW "%s a ete supprime\n", argv[2]);
    }
    else if(!strcmp(argv[1], "add")){
        if(argc < 3){
            printf("Utilisation : %s add <elem> [<elem2> <elem3> ...]\n", argv[0]);
            return 0;
        }

        for(int i = 2; i < argc; i++){
            myGitAdd(argv[i]);
            printf(YELLOW "%s a ete ajoute au git\n", argv[i]);
        }
    }
    else if(!strcmp(argv[1], "clear-add")){
        remove(".add");
        printf("Le fichier .add a ete supprime\n");
    }
    else if(!strcmp(argv[1], "add-list")){
        printf("Zone de preparation :\n");
        if(file_exists(".add")){
            WorkTree *wt = ftwt(".add");
            printf("%s\n", wtts(wt));
        }
    }
    else if(!strcmp(argv[1], "commit")){
        if(argc < 3){
            printf("Utilisation : %s commit <branch name> [-m <message>]\n", argv[0]);
            return 0;
        }

        char *name = NULL;
        char *msg = NULL;

        for(int i = 2; i < argc; i++){
            if(argv[i][0] == '-') continue; //Si le paramètre commence par un tiret
            else if(!strcmp(argv[i-1], "-m")) msg = argv[i]; //Si il est précédé de -m il s'agit du message
            else name = argv[i]; // Sinon il s'agit du nom de la branche
        }
        
        if(name == NULL){
            printf("Utilisation : %s commit <branch name> [-m <message>]\n", argv[0]);
            printf("Les paramètres entre crochets sont obligatoires\n");
            return 0;
        }

        myGitCommit(name, msg);
    }
    else if(!strcmp(argv[1], "test")){
        //TODO : Rajouter les fonctions de tests
    }
    else {
        printf("Commande %s inconnue, arrêt du programme...", argv[1]);
        return -1;
    }
    

    // FIN DU PROGRAMME
    puts("Fin du programme");
    return 0;
}


