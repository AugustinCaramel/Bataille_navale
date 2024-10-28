#ifndef JOUEUR_H
#define JOUEUR_H

#include <stdbool.h>
#include "navire.h"

typedef struct
{
    int indice;               // 1 ou 2, pour simplifier l'affichage des grilles
    char grille[10][10];      // Grille qui contient les vraies données sur les navires du joueur
    char grille_tirs[10][10]; // Grille qui contient les données sur les précédentes tentatives de tirs du joueur
    Navire navires[5];        // Liste des 5 navires du joueur
    char nom[30];             // Nom du joueur, demandé en début de partie
} Joueur;

void remplir_grille(char grille[10][10]);

#endif // JOUEUR_H
