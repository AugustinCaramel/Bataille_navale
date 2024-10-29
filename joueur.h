#ifndef JOUEUR_H
#define JOUEUR_H

#include "navire.h"



typedef struct
{
    int indice;               // 1 ou 2, pour simplifier l'affichage des grilles
    char grille[10][10];      // Grille qui contient les vraies données sur les navires du joueur
    char grille_tirs[10][10]; // Grille qui contient les données sur les précédentes tentatives de tirs du joueur
    Navire navires[5];        // Liste des 5 navires du joueur
    char nom[30];             // Nom du joueur, demandé en début de partie
} Joueur;


void afficher_les_noms(Joueur *attaquant, Joueur *defenseur);
void afficher_grilles(Joueur *attaquant, Joueur *defenseur);
void maj_grille_tirs(Joueur *defenseur, Joueur *attaquant, int indice);
bool etat_navire(Joueur *defenseur, int indice_navire, int indiceY, int indiceX);
void update_navires(Joueur *attaquant, Joueur *defenseur);
bool joueur_a_perdu(Joueur *joueur);
void placer_navires(Joueur *joueur, char select[3]);
Joueur initialiser_joueur(char select[3], int indice);
void placer_navires_IA(Joueur *joueur, char select[3]);
Joueur initialiser_joueur_IA(char select[3], int indice);

#endif // JOUEUR_H