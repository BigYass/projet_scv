# Copie de Github
### Devoir pour la Sorbonne université
Une copie de [git](https://git-scm.com/) (controleur de source) avec des fonctionnalité primaire pour la Sorbonne Université.

## Compilation et aides
#### Compilation
```
make
```

#### Lancer 
```
make run
```

#### Nettoyer
```
make clean
```
#### Utilisation
```
bin/myGit init : initialise le repertoire de references.\n, 
bin/myGit list-refs : affiche toutes les references existantes.\n, 
bin/myGit create-ref <name> <hash> : cree la reference <name> qui pointe vers le commit correspondant au hash donne.\n, 
bin/myGit delete-ref <name> : supprime la reference name.\n, 
bin/myGit add <elem> [<elem2> <elem3> ...] : ajoute un ou plusieurs fichiers/repertoires la la zone de preparation (pour faire partie du prochain commit).\n, 
bin/myGit list-add : affiche le contenu de la zone de preparation.\n, 
bin/myGit clear-add : vide la zone de preparation.\n, 
bin/myGit commit <branch-name> [-m <message>] : effectue un commit sur une branche, avec ou sans message descriptif.\n, 
bin/myGit get-current-branch : Affiche le nom de la branche courrante\n, 
bin/myGit branch <branch-name>: Crée la branche de nom <branch-name>\n, 
bin/myGit branch-print <branch-name>: Affiche le hash de tous les commits de la branche\n, 
bin/myGit checkout-branch <branch-name>: Réalise un déplacement de branche\n, 
bin/myGit checkout-commit <pattern>: Réalise un déplacement de commit qui commence par <pattern>\n, 


```