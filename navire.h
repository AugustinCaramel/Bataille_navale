#ifndef NAVIRE_H
#define NAVIRE_H

#include "utilitaries.h"

typedef struct
{
    int longueur, pos_y, pos_x;
    char type[15];    // Torpilleur, croiseur, etc.
    bool etat;        // Indicateur d'état
    char orientation; // N pour Nord, E pour Est, S pour Sud, O pour Ouest
} Navire;



// Les fonctions prennent maintenant des pointeurs vers struct Joueur
Navire creer_navire(int indice_navire, char nom[30], char grille[10][10], char select[3], bool aleatoire);
Navire creer_navire_IA(int indice_navire, int indice, char grille[10][10], char select[3], bool aleatoire);

#endif // NAVIRE_H
