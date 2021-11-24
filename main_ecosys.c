#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"


#define NB_PROIES 20
#define NB_PREDATEURS 25
#define T_WAIT 40000


int main(void) {
    srand(time(NULL));
 
    int i, j;

    int population_proies[301];
    int population_preds[301];

    Animal* pro = NULL;
    Animal* pre = NULL;

    Animal** proies = &pro;
    Animal** preds  = &pre;

    int herbe[SIZE_X][SIZE_Y];
    for (i = 0; i < SIZE_X; i++) {
        for (j = 0; j < SIZE_Y; j++) {
            herbe[i][j] = 0;
        }
    }

    for (i = 0; i < NB_PROIES; i++) {
        ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, rand() % 50 + 1, proies);
    }

    for (i = 0; i < NB_PREDATEURS; i++) {
        ajouter_animal(rand() % SIZE_X, rand() % SIZE_Y, rand() % 150 + 1, preds);
    }

    population_proies[0] = NB_PROIES;
    population_preds[0] = NB_PREDATEURS;


    // ================================
    for (i = 1; i < 301; i++) {
        usleep(T_WAIT);
        clear_screen();
        afficher_ecosys(*proies, *preds);
        rafraichir_proies(proies, herbe);
        rafraichir_predateurs(preds, proies);
        rafraichir_monde(herbe);
        population_proies[i] = compte_animal_it(*proies);
        population_preds[i] = compte_animal_it(*preds);
        printf("(Iteration n°%d)\n", i);
    }
    // ================================


    FILE *file = fopen("Evol_Pop.txt", "w");

    if (file == NULL) {
        printf("Erreur !");
        return 0;
    }

    for (i = 0; i < 301; i++) {
        fprintf(file, "%d %d %d\n", i, population_proies[i], population_preds[i]);
    }

    fclose(file);

    liberer_liste_animaux(*proies);
    liberer_liste_animaux(*preds);

    return 0;
}
