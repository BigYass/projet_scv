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
#include "include/org/branch.h"
#include "include/org/commit.h"


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
        printf(YELLOW "%s add <elem> [<elem2> <elem3> ...] :"RESET" ajoute un ou plusieurs fichiers/repertoires la la zone de preparation (pour faire partie du prochain commit).\n", argv[0]);
        printf(YELLOW "%s list-add :"RESET" affiche le contenu de la zone de preparation.\n", argv[0]);
        printf(YELLOW "%s clear-add :"RESET" vide la zone de preparation.\n", argv[0]);
        printf(YELLOW "%s commit <branch-name> [-m <message>] :"RESET" effectue un commit sur une branche, avec ou sans message descriptif.\n", argv[0]);
        printf(YELLOW "%s get-current-branch :"RESET" Affiche le nom de la branche courrante\n", argv[0]);
        printf(YELLOW "%s branch <branch-name>:"RESET" Crée la branche de nom <branch-name>\n", argv[0]);
        printf(YELLOW "%s branch-print <branch-name>:"RESET" Affiche le hash de tous les commits de la branche\n", argv[0]);
        printf(YELLOW "%s checkout-branch <branch-name>:"RESET" Réalise un déplacement de branche\n", argv[0]);
        printf(YELLOW "%s checkout-commit <pattern>:"RESET" Réalise un déplacement de commit qui commence par <pattern>\n", argv[0]);
        

        printf(YELLOW "%s test :"RESET" Lance des tests sur les fonctionnalites.\n", argv[0]);

        return 0;
    }


    create_tmp_file();

    if (!strcmp(argv[1], "init")){
        initRefs();
        initBranch();
        printf("Projet initialisé!\n");
    }
    else if(!strcmp(argv[1], "list-refs")){
        if(!file_exists(".refs")){
            printf("Le fichier .refs n'existe. Veuiller initialiser le programme avec la commande %s init\n", argv[0]);
            return 0;
        }
        printf("refs :\n");

        List *L = listdir(".refs");
        for(Cell* cursor = *L; cursor; cursor = cursor->next){
            if (cursor->data[0] == '.') continue;
            char *content = getRef(cursor->data);
            printf("- %s :\t\t %s\n", cursor->data, content);
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
            printf(YELLOW "%s" RESET " a ete ajoute au git\n", argv[i]);
        }
    }
    else if(!strcmp(argv[1], "clear-add")){
        remove(".add");
        printf("Le fichier .add a ete supprime\n");
    }
    else if(!strcmp(argv[1], "list-add")){
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
        size_t size = sizeof(char) * MAX_BUF_SIZE;
        char *msg = malloc(size);

        memset(msg, 0, size);

        bool is_message = false;

        for(int i = 2; i < argc; i++){
            if(argv[i][0] == '-') { //Si le paramètre commence par un tiret
                continue;
                if(!strcmp(argv[i-1], "-m")){
                    is_message = true;
                }
            } 
            else if(is_message){ //Si il est précédé de -m il s'agit du message
                if(strlen(msg) + strlen(argv[i]) >= size){
                    size <<= 1;
                    msg = realloc(msg, size);
                }
                strcat(msg, argv[i]);
            }
            else name = argv[i]; // Sinon il s'agit du nom de la branche
        }
        
        if(name == NULL){
            printf("Utilisation : %s commit <branch name> [-m <message>]\n", argv[0]);
            printf("Les paramètres entre crochets sont obligatoires\n");
            return 0;
        }

        myGitCommit(name, msg);
        printf("Le commit " YELLOW "\"%s\"" RESET " a été enregistré dans la branche " YELLOW "%s" RESET "\n", msg, name);
    }
    else if(!strcmp(argv[1], "get-current-branch")){
        printf("La branche courrante est "YELLOW"%s"RESET"\n", getCurrentBranch());
    }
    else if(!strcmp(argv[1], "branch")){
        if(argc < 3){
            printf(RED"Erreur de syntax, "RESET"Utilisation : %s branch <nom>\n", argv[0]);
        }
        else if(branchExists(argv[2])){
            printf("La branche "YELLOW"%s"RESET" existe déjà...\n", argv[2]);
        }
        else {
            createBranch(argv[2]);
            printf("La branche "YELLOW"%s"RESET" a été crée...\n", argv[2]);
        }
    }
    else if(!strcmp(argv[1], "branch-print")){
        if(argc < 3){
            printf(RED"Erreur de syntax, "RESET"Utilisation : %s branch-print <branch-name>\n", argv[0]);
        }
        else if(!branchExists(argv[2])){
            printf("La branche "YELLOW"%s"RESET" n'existe pas...\n", argv[2]);
        }
        else {
            printBranch(argv[2]);
        }
    }
    else if(!strcmp(argv[1], "checkout-branch")){
        if(argc < 3){
            printf(RED"Erreur de syntax, "RESET"Utilisation : %s checkout-branch <branch-name>\n", argv[0]);
        }
        else if(!branchExists(argv[2])){
            printf("La branche "YELLOW"%s"RESET" n'existe pas...\n", argv[2]);
        }
        else {
            myGitCheckoutBranch(argv[2]);
            printf("Déplacement de branche vers " YELLOW "%s" RESET, argv[2]);
        }
    }
    else if(!strcmp(argv[1], "checkout-commit")){
        if(argc < 3){
            printf(RED"Erreur de syntax, "RESET"Utilisation : %s checkout-commit <commit-name>\n", argv[0]);
        }
        else {
            myGitCheckoutCommit(argv[2]);
            printf("Déplacement de commit vers " YELLOW "%s" RESET, argv[2]);
        }
    }
    
    else if(!strcmp(argv[1], "test")){
        //TODO : Rajouter les fonctions de tests
    }
    else {
        printf("Commande %s inconnue, arrêt du programme...\n", argv[1]);
        return 0;
    }
    

    // FIN DU PROGRAMME
    puts(GRAY"Fin du programme...");
    return 0;
}


