#ifndef GAME_H
#define GAME_H

#include "joueur.h"

extern char saisie[3]; // Variable globale pour les saisies

typedef struct
{
    int num_damier; // pair pair = 0, pair impair = 1
    int nbr_tir;
    int coordonee_case[50][2]; // doordonnee de la case
    int nbr_case_vide[50]; // nombre de case vide adjasente de la case
} damier;

damier initialiser_damier(int num_damier);
void menu_principal(); // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
void choix_niveau(int *niveau); // Affiche les différents niveaux et modifie la valeur de niveau avec la valeur saisie par l'utilisateur
void lancer_partie(int niveau); // Initialise les deux joueurs et lance les tours
void lancer_tours(Joueur *joueur1, Joueur *joueur2, int niveau); // Gère les tours de jeu (la fonction n'est pas quittée tant qu'une partie est toujours en cours)

/* Fonctions de tir différenciées par niveau */

bool tirer(Joueur *attaquant, Joueur *defenseur,Cible *cible, int niveau); // Fonction de tir pour les joueurs humain
bool tirer_IA1(Joueur *attaquant, Joueur *defenseur, Cible *cible, int niveau); // Mode de jeu IA 1 : Tir aléatoire
bool tirer_IA2(Joueur *attaquant, Joueur *defenseur);
bool tirer_IA3(Joueur *attaquant, Joueur *defenseur);
bool mode_chasse_IA2(Joueur *attaquant,int *x,int *y,int *last_x,int *last_y);
bool mode_chasse_IA3(Joueur *attaquant,int *x,int *y,int *last_x,int *last_y);
void mode_reperage(Joueur *attaquant,int *x,int *y);
void genere_liste_tir_optimal(damier damier,int liste_tir[50],int *taille_max);
void genere_probabilite_case(char grille_tirs[10][10],int nbr_case_vide,int ligne,int colone); 
void genere_liste_case_vide(damier *damier_0,damier *damier_1,char grille_tirs[10][10]); // crée des tableau pour les deu damier et determine le nombre de case vide pour chaque case
void mode_reperage_IA3(Joueur *attaquant,int *x,int *y);
bool verifie_tire_touche_navire(Joueur *attaquant, Joueur *defenseur,int x,int y);

#endif // GAME_H
