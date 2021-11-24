#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ecosys.h"


#define NB_PROIES 20
#define NB_PREDATEURS 20


int main() {
    srand(time(NULL));

    int i;

    Animal* pro = NULL;
    Animal* pre = NULL;

    Animal** proies = &pro;
    Animal** preds  = &pre;

    for (i = 0; i < 4; i++) {
        ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, rand(), proies);
    }

    for (i = 0; i < 4; i++) {
        ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, rand(), preds);
    }

    afficher_ecosys(*proies, *preds);

    liberer_liste_animaux(*proies);
    liberer_liste_animaux(*preds);

    return 0;
}
