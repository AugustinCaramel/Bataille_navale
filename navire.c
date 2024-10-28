#include "navire.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int est_coordonnees(char select[3])
{
    return strlen(select) == 3 && isdigit(select[0]) && select[1] == '-' && isdigit(select[2]);
}

Navire creer_navire(int indice_navire, Joueur *joueur, char select[3], bool aleatoire) {
    // Implémentation de creer_navire
}

Navire creer_navire_IA(int indice_navire, Joueur *joueur, char select[3], bool aleatoire) {
    // Implémentation de creer_navire_IA
}

