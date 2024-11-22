#ifndef GAME_H
#define GAME_H

#include "joueur.h"

extern char saisie[3]; // Variable globale pour les saisies

typedef struct
{
    bool etat; // Actif : mode cible, Inactif : mode chasse
    int touche_position[20][2];
    bool orientation[4];
    int nombre_touches;

} Cible;


void menu_principal(); // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
void choix_niveau(int *niveau); // Affiche les différents niveaux et modifie la valeur de niveau, avec la valeur de saisie
void lancer_partie(bool IA, int niveau); // Initialise les deux joueurs et lance les tours
void lancer_tours(Joueur *joueur1, Joueur *joueur2, bool IA, int niveau);
bool tirer(Joueur *attaquant, Joueur *defenseur);
bool tirer_IA1(Joueur *attaquant, Joueur *defenseur);
bool tirer_IA2(Joueur *attaquant, Joueur *defenseur);
bool tirer_IA3(Joueur *attaquant, Joueur *defenseur, Cible *cible);
void decaler_gauche(Cible *cible, int indice);
void actualiser_cible(Cible *cible,Navire navire);

#endif // GAME_H
