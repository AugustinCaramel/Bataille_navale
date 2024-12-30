#ifndef GAME_H
#define GAME_H

#include "joueur.h"

extern char saisie[3]; // Variable globale pour les saisies

void menu_principal(); // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
void choix_niveau(int *niveau); // Affiche les différents niveaux et modifie la valeur de niveau avec la valeur saisie par l'utilisateur
void lancer_partie(int niveau); // Initialise les deux joueurs et lance les tours
void lancer_tours(Joueur *joueur1, Joueur *joueur2, int niveau); // Gère les tours de jeu (la fonction n'est pas quittée tant qu'une partie est toujours en cours)

/* Fonctions de tir différenciées par niveau */

bool tirer(Joueur *attaquant, Joueur *defenseur,Cible *cible, int niveau); // Fonction de tir pour les joueurs humain
bool mode_chasse_IA2(Joueur *attaquant,int *x,int *y,int *last_x,int *last_y);
void mode_reperage(Joueur *attaquant,int *x,int *y);

#endif // GAME_H
