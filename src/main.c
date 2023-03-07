#include <stdio.h>
#include <stdlib.h>

#include "const.h"
#include "hash.h"
#include "liste.h"

int main(int argc, char *argv[])
{

    if(argc < 1)
        return 0;

    const char* filename = "Makefile";
    
    printf("Hash de %s : ", filename);
    printf("%s\n\n", sha256file(filename));

    puts("Creation de la liste et des cellules...\n");
    List* l = initList();
    Cell* c1 = buildCell("burger");
    Cell* c2 = buildCell("cheese");
    insertFirst(l,c1);
    insertFirst(l,c2);

    puts("Conversion de la liste en chaine de character...\n");
    char* s = ltos(l);

    printf("Conversion : %s\n",s);
    List* l2 = stol(s);
    
    puts("Affichage de la structure :\n");
    Cell * cursor = *l2;

    printf("Résultat : %s\n", list_string(l2));
    
    return 0;
}


