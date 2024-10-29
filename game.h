#ifndef GAME_H
#define GAME_H

#include "joueur.h"

void menu_principal(char select[3]);
void lancer_partie(char select[3]);
void partie_IA_1(char select[3]);
void choix_niveau(char select[3]);
void lancer_tours(Joueur *joueur1, Joueur *joueur2, char select[3], bool IA);
bool tirer(Joueur *attaquant, Joueur *defenseur, char select[3]);
bool tir_utile(int x, int y, Joueur *attaquant);
bool tirer_IA_1(Joueur *attaquant, Joueur *defenseur, char select[3]);

#endif // GAME_H
