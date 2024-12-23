#ifndef GAME_H
#define GAME_H

#include "joueur.h"

extern char saisie[3]; // Variable globale pour les saisies

void menu_principal(); // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
void choix_niveau(int *niveau); // Affiche les différents niveaux et modifie la valeur de niveau avec la valeur saisie par l'utilisateur
void lancer_partie(int niveau); // Initialise les deux joueurs et lance les tours
void lancer_tours(Joueur *joueur1, Joueur *joueur2, int niveau); // Gère les tours de jeu (la fonction n'est pas quittée tant qu'une partie est toujours en cours)

/* Les fonctions ci-dessous sont utilisées pour le mode de jeu IA 3 */

int calculerPonderation(int y, int x); // Retourne une pondération aléatoire pour chaque case "à chasser", les cases centrales ayant une pondération plus faible
void melangerAvecPriorite(int chasse[50][2], int taille_chasse); // Mélange les coordonnées de chasse aléatoirement en priorisant les cases centrales
void trouver_case_adjacente(int orientation, int *y, int *x, Cible *cible, Joueur *attaquant, int niveau); // Trouve une case à cibler en fonction des dernières positions touchées

/* Fonctions de tir différenciées par niveau */

bool tirer(Joueur *attaquant, Joueur *defenseur,Cible *cible, int niveau); // Fonction de tir pour les joueurs humains
bool tirer_IA1(Joueur *attaquant, Joueur *defenseur,Cible *cible, int niveau); // Mode de jeu IA 1 : Tir aléatoire
bool tirer_IA2(Joueur *attaquant, Joueur *defenseur, Cible *cible, int niveau); // Mode de jeu IA 2 : Tir aléatoire, mais avec ciblage des cases adjacentes si un navire est touché
bool tirer_IA3(Joueur *attaquant, Joueur *defenseur, Cible *cible,int niveau, int chasse[50][2], int *taille_chasse); // Mode de jeu IA 3 : Stratégies de chasse et de cible avancées

#endif // GAME_H
