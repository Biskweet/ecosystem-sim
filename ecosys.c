#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"


float p_ch_dir = 0.01;
float p_reproduce_proie = 0.4;
float p_reproduce_predateur = 0.5;
int temps_repousse_herbe = -15;


/* PARTIE 1*/
/* Fourni: Part 1, exercice 3, question 2 */
Animal *creer_animal(int x, int y, float energie) {
    Animal *na = (Animal *)malloc(sizeof(Animal));
    assert(na);
    na->x = x;
    na->y = y;
    na->energie = energie;
    na->dir[0] = rand() % 3 - 1;
    na->dir[1] = rand() % 3 - 1;
    na->suivant = NULL;
    return na;
}


/* Fourni: Part 1, exercice 3, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
    assert(animal);
    assert(!animal->suivant);
    animal->suivant = liste;
    return animal;
}

/* A faire. Part 1, exercice 5, question 1 */
void ajouter_animal(int x, int y, float energie, Animal **liste_animal) {
    assert(0 <= x && x < SIZE_X);
    assert(0 <= y && y < SIZE_Y);

    Animal* ani = creer_animal(x, y, energie);

    ani = ajouter_en_tete_animal(*liste_animal, ani);
    *liste_animal = ani;
}

/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
    Animal* ani = *liste;

    if (ani == NULL) return;

    if (ani == animal) {
        *liste = ani->suivant;
        free(ani);
        return;
    }

    while (ani->suivant != NULL) {
        if (ani->suivant == animal) {
            Animal* temp = ani->suivant;
            ani->suivant = temp->suivant;
            free(temp);
            return;
        }

        ani = ani->suivant;
    }
}

/* A Faire. Part 1, exercice 5, question 2 */
Animal* liberer_liste_animaux(Animal *liste) {
    Animal* temp = liste;

    while (liste != NULL) {
        liste = liste->suivant;
        free(temp);
        temp = liste;
    }

    return NULL;
}


/* Fourni: part 1, exercice 3, question 4 */
unsigned int compte_animal_rec(Animal *la) {
    if (!la) return 0;
    return 1 + compte_animal_rec(la->suivant);
}


/* Fourni: part 1, exercice 3, question 4 */
unsigned int compte_animal_it(Animal *la) {
    int cpt=0;
    while (la) {
        ++cpt;
        la = la->suivant;
    }
    return cpt;
}


/* Part 1. Exercice 4, question 1 */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
    unsigned int i, j;
    char ecosys[SIZE_X][SIZE_Y];

    printf("Nombre proies : %d\nNombre predateurs : %d\n", compte_animal_rec(liste_proie), compte_animal_rec(liste_predateur));

    // On initialise le tableau
    for (i = 0; i < SIZE_X; i++) {
        for (j = 0; j < SIZE_Y; j++) {
            ecosys[i][j] = ' ';
        }
    }

    // On ajoute les proies
    while (liste_proie != NULL) {
        ecosys[liste_proie->x][liste_proie->y] = '*';
        liste_proie = liste_proie->suivant;
    }

    // On ajoute les predateurs
    while (liste_predateur != NULL) {
        if (ecosys[liste_predateur->x][liste_predateur->y] == '*') {
            ecosys[liste_predateur->x][liste_predateur->y] = '@';
        }
        else {
            ecosys[liste_predateur->x][liste_predateur->y] = 'O';
        }
        liste_predateur = liste_predateur->suivant;
    }

    // On affiche le tableau
    printf("+"); for (int i = 0; i < SIZE_Y; i++) {printf("-");} printf("+\n");   
    for (int i = 0; i < SIZE_X; i++) {
        printf("|");
        for (int j = 0; j < SIZE_Y; j++) {
            printf("%c", ecosys[i][j]);
        }
        printf("|\n");
    }
    printf("+"); for (int i = 0; i < SIZE_Y; i++) {printf("-");} printf("+\n");
}


void clear_screen() {
    printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
    float alea;


    while (la != NULL) {
        alea = (float) rand() / RAND_MAX;
        if (alea < p_ch_dir) {
            la->dir[0] = (int) (rand() % 3 - 1);
        }

        alea = (float) rand() / RAND_MAX;
        if (alea < p_ch_dir) {
            la->dir[1] = (int) (rand() % 3 - 1);
        }
        
        la->x += la->dir[0];
        la->y += la->dir[1];

        if (la->x >= SIZE_X) {
            la->x = SIZE_X - la->x;
        }

        if (la->x < 0) {
            la->x = SIZE_X - 1;
        }

        if (la->y >= SIZE_Y) {
            la->y = SIZE_Y - la->y;
        }

        if (la->y < 0) {
            la->y = SIZE_Y - 1;
        }
    
        la = la->suivant;
    }

}

/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce) {
    Animal *ani = *liste_animal;
    float alea;

    while (ani != NULL) {
        alea = (float) rand() / RAND_MAX;
        if (alea < p_reproduce) {
            ani->energie /= 2;
            ajouter_animal(ani->x, ani->y, ani->energie, liste_animal);
        }

        ani = ani->suivant;
    }
}


/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
    while (l != NULL) {
        if ((l->x == x) && (l->y == y)) {
            return l;
        }

        l = l->suivant;
    }

    return NULL;
} 


/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    Animal *copie = *liste_proie;
    Animal* temp;

    bouger_animaux(*liste_proie);

    while (copie != NULL) {
        copie->energie -= 1;
        if (monde[copie->x][copie->y] > 0) {
            copie->energie += monde[copie->x][copie->y];
            monde[copie->x][copie->y] = temps_repousse_herbe;
        }

        if (copie->energie < 0) {
            temp = copie;
            copie = copie->suivant;
            enlever_animal(liste_proie, temp);
        }

        else {
            copie = copie->suivant;
        }
    }

    reproduce(liste_proie, p_reproduce_proie);
}


/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
    Animal *copie = *liste_predateur;
    Animal *proie, *temp;
    
    bouger_animaux(*liste_predateur);

    while (copie != NULL) {
        copie->energie -= 1;
        proie = animal_en_XY(*liste_proie, copie->x, copie->y); 
        if (proie != NULL) {
            copie->energie += proie->energie + 1;
            enlever_animal(liste_proie, proie);
        }

        if (copie->energie < 0) {
            temp = copie;
            copie = copie->suivant;
            enlever_animal(liste_predateur, temp);
        }

        else {
            copie = copie->suivant;
        }
    }

    reproduce(liste_predateur, p_reproduce_predateur);
}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
    for (int i = 0; i < SIZE_X; i++) {
        for (int j = 0; j < SIZE_Y; j++) {
            monde[i][j] += 1;
        }
    }
}

