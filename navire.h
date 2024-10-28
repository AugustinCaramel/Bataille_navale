#ifndef NAVIRE_H
#define NAVIRE_H

#include <stdbool.h>
#include "joueur.h"

typedef struct
{
    int longueur, pos_y, pos_x;
    ;
    char type[15];    // Torpilleur, croiseur, etc.
    bool etat;        // Me sert pour optimiser quelques fonctions, mais ne joue pas un rôle clé dans mon programme
    char orientation; // N pour Nord, E pour Est, S pour Sud, O pour Ouest
} Navire;             // Structure qui me permet de simplifier l'accès aux données des différents navires

int est_coordonnees(char select[3]);
Navire creer_navire(int indice_navire, Joueur *joueur, char select[3], bool aleatoire);
Navire creer_navire_IA(int indice_navire, Joueur *joueur, char select[3], bool aleatoire);

#endif // NAVIRE_H