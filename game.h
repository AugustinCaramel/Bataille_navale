#ifndef GAME_H
#define GAME_H

#include "joueur.h"

extern char saisie[3]; // Variable globale pour les saisies

void menu_principal(); // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
void choix_niveau(int *niveau); // Affiche les différents niveaux et modifie la valeur de niveau, avec la valeur de saisie
void lancer_partie(bool IA, int niveau); // Initialise les deux joueurs et lance les tours
void lancer_tours(Joueur *joueur1, Joueur *joueur2, bool IA, int niveau);
bool tirer(Joueur *attaquant, Joueur *defenseur);
bool tirer_1(Joueur *attaquant, Joueur *defenseur);

#endif // GAME_H
