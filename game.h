#ifndef GAME_H
#define GAME_H

#include "joueur.h"

extern char saisie[3]; // Variable globale pour les saisies

void menu_principal(); // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
void choix_niveau(int *niveau); // Affiche les différents niveaux et modifie la valeur de niveau avec la valeur saisie par l'utilisateur
void lancer_partie(int niveau); // Initialise les deux joueurs et lance les tours
void lancer_tours(Joueur *joueur1, Joueur *joueur2, int niveau); // Gère les tours de jeu (la fonction n'est pas quittée tant qu'une partie est toujours en cours)

/* Fonctions de tir différenciées par niveau */

bool tirer(Joueur *attaquant, Joueur *defenseur,Cible *cible, int niveau); // Fonction de tir pour les joueurs humains
bool tirer_IA1(Joueur *attaquant, Joueur *defenseur,Cible *cible, int niveau); // Mode de jeu IA 1 : Tir aléatoire

#endif // GAME_H
