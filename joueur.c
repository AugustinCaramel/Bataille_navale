#include "joueur.h"

void remplir_grille(char grille[10][10]) {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            grille[i][j] = '.';
        }
    }
}
