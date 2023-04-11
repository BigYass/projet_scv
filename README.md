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
bin/myGit init : initialise le repertoire de references. 
```
```
bin/myGit list-refs : affiche toutes les references existantes. 
```
```
bin/myGit create-ref <name> <hash> : cree la reference <name> qui pointe vers le commit correspondant au hash donne. 
```
```
bin/myGit delete-ref <name> : supprime la reference name. 
```
```
bin/myGit add <elem> [<elem2> <elem3> ...] : ajoute un ou plusieurs fichiers/repertoires la la zone de preparation (pour faire partie du prochain commit). 
```
```
bin/myGit list-add : affiche le contenu de la zone de preparation. 
```
```
bin/myGit clear-add : vide la zone de preparation. 
```
```
bin/myGit commit <branch-name> [-m <message>] : effectue un commit sur une branche, avec ou sans message descriptif. 
```
```
bin/myGit get-current-branch : Affiche le nom de la branche courrante 
```
```
bin/myGit branch <branch-name>: Crée la branche de nom <branch-name> 
```
```
bin/myGit branch-print <branch-name>: Affiche le hash de tous les commits de la branche 
```
```
bin/myGit checkout-branch <branch-name>: Réalise un déplacement de branche 
```
```
bin/myGit checkout-commit <pattern>: Réalise un déplacement de commit qui commence par <pattern> 
```