#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <ctype.h>

#include "include/const.h"

#include "include/test/test.h"
#include "include/test/debug.h"

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
        printf(YELLOW "%s init :"RESET" Initialise le repertoire de references.\n", argv[0]);
        printf(RED "%s list-refs :"RESET" Affiche toutes les references existantes.\n", argv[0]);
        printf(YELLOW "%s create-ref <name> <hash> :"RESET" Cree la reference <name> qui pointe vers le commit correspondant au hash donne.\n", argv[0]);
        printf(RED "%s delete-ref <name> :"RESET" Supprime la reference name.\n", argv[0]);
        printf(YELLOW "%s add <elem> [<elem2> <elem3> ...] :"RESET" Ajoute un ou plusieurs fichiers/repertoires a la zone de preparation (pour faire partie du prochain commit).\n", argv[0]);
        printf(RED "%s list-add :"RESET" Affiche le contenu de la zone de preparation.\n", argv[0]);
        printf(YELLOW "%s clear-add :"RESET" Vide la zone de preparation.\n", argv[0]);
        printf(RED "%s commit <branch-name> [-m <message>] :"RESET" Effectue un commit sur une branche, avec ou sans message descriptif.\n", argv[0]);
        printf(YELLOW "%s get-current-branch :"RESET" Affiche le nom de la branche courrante\n", argv[0]);
        printf(RED "%s branch <branch-name>:"RESET" Crée la branche de nom <branch-name>\n", argv[0]);
        printf(YELLOW "%s branch-print <branch-name>:"RESET" Affiche le hash de tous les commits de la branche\n", argv[0]);
        printf(RED "%s checkout-branch <branch-name>:"RESET" Réalise un déplacement de branche\n", argv[0]);
        printf(YELLOW "%s checkout-commit <pattern>:"RESET" Réalise un déplacement de commit qui commence par <pattern>\n", argv[0]);
        printf(RED "%s merge <branch> [message]:"RESET" Fusionne la branche courrante et <branch>\n", argv[0]);
        printf(YELLOW "%s test [<module1> <module2> ...] :"RESET" Lance des tests sur les fonctionnalites des modules spécifique (de tout les modules si non spécifiés).\n", argv[0]); 
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
        printf("La reference " YELLOW "%s" RESET " a ete supprime\n", argv[2]);
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
                if(strcmp(argv[i-1], "-m")){
                    is_message = true;
                    err_log(E_OK, "Paramètre -m détecté, changement de mode");
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
            printf("Réstauration de la branche " YELLOW "%s" RESET "\n", argv[2]);
        }
    }
    else if(!strcmp(argv[1], "checkout-commit")){
        if(argc < 3){
            printf(RED"Erreur de syntax, "RESET"Utilisation : %s checkout-commit <commit-name>\n", argv[0]);
        }
        else {
            myGitCheckoutCommit(argv[2]);
            printf("Déplacement de commit vers " YELLOW "%s" RESET "\n", argv[2]);
        }
    }
    else if(!strcmp(argv[1], "merge")){
        if(argc < 3){
            printf(RED "Erreur de syntax, " RESET "Utilisation : %s merge <branch> [message]\n", argv[0]);
        }
        else if(!branchExists(argv[2])){
            printf(RED" La branche " YELLOW "%s" RED " n'existe pas..\n" RESET, argv[2]); 
        }
        else {
            char *current_branch = getCurrentBranch();

            size_t size = sizeof(char) * MAX_BUF_SIZE;
            char *msg = malloc(size);
            memset(msg, 0, size);

            char delim[2] = "\0";

            for(int i = 2; ++i < argc;*delim = ' '){
                if(size >= strlen(msg) + strlen(argv[i])){
                    size <<= 2; msg = realloc(msg, size); // => size *= 2
                }
                strcat(msg, argv[i]);
                strcat(msg, delim);
            }

            List *conflitcs = merge(argv[2], strlen(msg) ? msg : NULL);

            if(conflitcs == NULL){
                printf("Fusion de la branche %s et %s accompli!\n", current_branch, argv[2]);
            }
            else {
                printf(RED "Impossible de fusionner, conflit :\n" RESET);
                printf(YELLOW "1." RESET " Garder les fichier de la branche courrante (%s)\n", current_branch);
                printf(YELLOW "2." RESET " Garder les fichier de l'autre branche (%s)\n", argv[2]);
                printf(YELLOW "3." RESET " Choisir manuellement les fichiers à garder (%d fichiers)\n", sizeList(conflitcs));
                printf(YELLOW "4." RESET " Annuler\n");
                printf("Votre choix : " YELLOW);

                int choice = -1;
                do {
                    scanf("%d", &choice);
                    printf(RESET);
                    if(choice < 1 || choice > 4){
                        printf("Veuillez entrez un nombre entre 1 et 4 : " YELLOW);
                        fflush(stdin);
                    }
                }while(choice < 1 || choice > 4);
                
                List *new_conflitcts = NULL;

                switch (choice)
                {
                
                case 1:
                    createDeletionCommit(argv[2], conflitcs, msg);
                    new_conflitcts = merge(current_branch, argv[2]);
                    printf("Vous avez choisis de garder les fichier de la branche courrante (%s). ", current_branch);

                    if(new_conflitcts == NULL){
                        printf("Les branches ont été correctement fusionné!\n");
                    }
                    else {
                        err_logf(E_ERR, "La branche %s et %s connaisse encore des conflits après la supression de celle ci\n", current_branch, argv[2]);
                    }

                    break;

                case 2:
                    createDeletionCommit(current_branch, conflitcs, msg);
                    new_conflitcts = merge(current_branch, argv[2]);
                    printf("Vous avez choisis de garder les fichier l'autre branche (%s). ", argv[2]);

                    if(new_conflitcts == NULL){
                        printf("Fin des conflits!\nLes branches ont été correctement fusionné!\n");
                    }
                    else {
                        err_logf(E_ERR, "La branche %s et %s connaisse encore des conflits après la supression de celle ci\n", current_branch, argv[2]);
                    }

                    break;

                case 3:
                {
                    List* remote_suppr = initList();
                    List* current_suppr = initList();

                    for(Cell *file = *conflitcs; file != NULL; file = file->next){
                        // INPUT
                        choice = -1;
                        printf("Pour le fichier %s, de quel branche garde-t-on le fichier ?\n1. %s\n2. %s\nVotre choix : ", file->data, current_branch, argv[2]);                        
                        do {
                            scanf("%d", &choice);
                            if(choice < 1 || choice > 2){
                                printf("Choix invalide, votre choix : ");
                                fflush(stdin);
                            }
                        }while(choice < 1 || choice > 2);

                        insertFirst((choice == 1 ? remote_suppr : current_suppr), buildCell(file->data));
                    }

                    createDeletionCommit(current_branch, current_suppr, msg);
                    createDeletionCommit(argv[2], remote_suppr, msg);
                    List *new_conflitcts = merge(current_branch, msg);

                    if(new_conflitcts == NULL){
                        printf("Fin des conflits!\nLes branches ont été correctement fusionné!\n");
                    }
                    else {
                        err_logf(E_ERR, "La branche %s et %s connaisse encore des conflits après la supression de celle ci", current_branch, argv[2]);
                    }

                    break;
                }

                case 4:
                    printf("Fusion annullé...\n");
                    break;
                
                default:
                    printf("Choix inconnue, annulation...\n");
                    break;
                }

            }
        }
    }
    else if(!strcmp(argv[1], "test")){
        Commit* modules = NULL;
        if(argc > 2){
            modules = initCommit();
            for(int i = 0; i < argc; i++){
                commitSet(modules, argv[i], "true");
            }
        }
        init_test();

        char *value = NULL;

        value = commitGet(modules, "hash");
        if(modules == NULL || value){
            test_hash();
            free(value);
        }
        value = commitGet(modules, "list");
        if(modules == NULL || value){
            test_list();
            free(value);
        }
        value = commitGet(modules, "file");
        if(modules == NULL || value){
            test_files();
            free(value);
        }
        value = commitGet(modules, "worktree");
        if(modules == NULL || value){
            test_work_file();
            free(value);
        }
        value = commitGet(modules, "branch");
        if(modules == NULL || value){
            test_branch();
            free(value);
        }
        value = commitGet(modules, "commit");
        if(modules == NULL || value){
            test_commit();
            free(value);
        }
        value = commitGet(modules, "git");
        if(modules == NULL || value){
            test_my_git();
            free(value);
        }
        value = commitGet(modules, "refs");
        if(modules == NULL || value){
            test_refs();
            free(value);
        }
        freeCommit(modules);
    }
    else {
        printf("Commande %s inconnue, arrêt du programme...\n", argv[1]);
        return 0;
    }

    // FIN DU PROGRAMME
    puts(GREEN "Fin du programme..." RESET);
    return 0;
}


