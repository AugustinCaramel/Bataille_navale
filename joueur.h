#ifndef JOUEUR_H
#define JOUEUR_H

#include "navire.h"

extern char saisie[3]; // Variable globale pour les saisies

typedef struct
{
    int indice;               // 1 ou 2, pour simplifier l'affichage des grilles
    char grille[10][10];      // Grille qui contient les vraies données sur les navires du joueur
    char grille_tirs[10][10]; // Grille qui contient les données sur les précédentes tentatives de tirs du joueur
    Navire navires[5];        // Liste des 5 navires du joueur
    char nom[30];             // Nom du joueur, demandé en début de partie
} Joueur;

Joueur initialiser_joueur(int indice, int niveau); // Initialise les attributs du joueur : grilles, nom, placement des navires, indice (avec l'indice passé en paramètre)
void placer_navires(Joueur *joueur, int niveau); // Définit le mode aléatoire ou non, crée les 5 navires et les positionne sur la grille
void afficher_grilles(Joueur *attaquant, Joueur *defenseur); // Affichage des grilles de tirs des deux joueurs côte-côte
void update_grille_tirs(Joueur *defenseur, Joueur *attaquant, int indice); // Remplace les X par des N sur un navire, sur une grille de tirs, si le navire est coulé
bool verifier_etat_navire(Joueur *defenseur, int indice_navire, int indiceY, int indiceX); // Renvoie false si le navire est entièrement touché
bool update_navires(Joueur *attaquant, Joueur *defenseur); // Vérifie l'ensemble de la flotte du joueur, met à jour les grilles de tirs et annonce si un nouveau navire est coulé
bool verifier_joueur_a_perdu(Joueur *joueur); // Vérifie que le joueur a encore >=1 navire actif

#endif // JOUEUR_H