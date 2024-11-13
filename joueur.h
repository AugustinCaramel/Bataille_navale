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

Joueur initialiser_joueur(char select[3], int indice, bool IA); // Initialise les attributs du joueur : grilles, nom, placement des navires, indice (avec l'indice passé en paramètre)
void placer_navires(Joueur *joueur, char select[3], bool IA); // Définit le mode aléatoire ou non, crée les 5 navires et les positionne sur la grille
void afficher_grilles(Joueur *attaquant, Joueur *defenseur);
void update_grille_tirs(Joueur *defenseur, Joueur *attaquant, int indice); // Remplace les X par des N sur un navire, sur une grille de tirs, si le navire est coulé
bool verifier_etat_navire(Joueur *defenseur, int indice_navire, int indiceY, int indiceX); // Renvoie false si le navire est entièrement touché
void update_navires(Joueur *attaquant, Joueur *defenseur); // Vérifie l'ensemble de la flotte du joueur, met à jour les grilles de tirs et annonce la déchéance du navire si un nouveau navire est coulé
bool verifier_joueur_a_perdu(Joueur *joueur); // Vérifie que le joueur a encore >=1 navire actif


#endif // JOUEUR_H