#ifndef GAME_H
#define GAME_H

#include "joueur.h"

void menu_principal(char select[3]); // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
void choix_niveau(char select[3], int *niveau); // Affiche les différents niveaux et modifie la valeur de niveau, avec la valeur de select
void lancer_partie(char select[3], bool IA, int niveau); // Initialise les deux joueurs et lance les tours
void lancer_tours(Joueur *joueur1, Joueur *joueur2, char select[3], bool IA, int niveau);
bool tirer(Joueur *attaquant, Joueur *defenseur, char select[3]);
bool tirer_1(Joueur *attaquant, Joueur *defenseur, char select[3]);

#endif // GAME_H
