#ifndef GAME_H
#define GAME_H

#include "joueur.h"

extern char saisie[3]; // Variable globale pour les saisies

typedef struct
{
    int num_damier; // pair pair = 0, pair impair = 1
    int nbr_case; // nombr de case vide
    int nbr_tir; // nombre de case ayant un tir optimal
    int coordonee_case[50][2]; // liste de doordonnee de chaque case, 0 = x, 1 = y
    int nbr_case_adjacente[50]; // liste avec nombre de case vide adjasente de chaque case
    int liste_tir[50]; // liste des numero de cases des tires optimal
} damier;

damier initialiser_damier(int num_damier); // initialisation des atribue de damier
void menu_principal(); // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
void choix_niveau(int *niveau); // Affiche les différents niveaux et modifie la valeur de niveau avec la valeur saisie par l'utilisateur
void lancer_partie(int niveau); // Initialise les deux joueurs et lance les tours
void lancer_tours(Joueur *joueur1, Joueur *joueur2, int niveau); // Gère les tours de jeu (la fonction n'est pas quittée tant qu'une partie est toujours en cours)

/* Fonctions de tir différenciées par niveau */

bool tirer(Joueur *attaquant, Joueur *defenseur,Cible *cible, int niveau); // Fonction de tir pour les joueurs humain
bool tirer_IA1(Joueur *attaquant, Joueur *defenseur, Cible *cible, int niveau); // Mode de jeu IA 1 : Tir aléatoire
bool tirer_IA2(Joueur *attaquant, Joueur *defenseur); // Mode de jeu IA 2 : Tir aléatoire et peut cibler un navire une fois detecté
bool tirer_IA3(Joueur *attaquant, Joueur *defenseur); // Mode de jeu IA 3 : Tir en damier sur la case avec le plus de case adjacente vide et traque un navire en interprétant sa direction
bool mode_chasse_IA2(Joueur *attaquant,int *x,int *y,int *last_x,int *last_y); // choisis au hazard un tir adjacent de la derniere case touché
bool mode_chasse_IA3(Joueur *attaquant,int *x,int *y,int *last_x,int *last_y); // choisis au hazard un tir adjacent de la derniere case touché, d'est que deux case touchée sont adjacente, il en déduit la direction du navire
void mode_reperage(Joueur *attaquant,int *x,int *y); // choisis un tir aléatoir
void choisi_tir_optimal(damier damier,int *x,int *y); // choisis au hazard une des case du damier avec le plus haut nombre de case adjacente vide et renvoye ces coordonnée
void genere_liste_tir_optimal(damier *damier,int *nbr_case_max);// crée une liste avec le numero de case de toute les case du damier ayant le nombre de case ajdacente vide maximal
void genere_probabilite_case(char grille_tirs[10][10],int *nbr_case_vide,int ligne,int colone,int taille_navire); // détermine le nombre de case vide dans les 4 direction de la case sélectionnée
void genere_liste_case_vide(damier *damier_0,damier *damier_1,char grille_tirs[10][10],int taille_navire); // crée des tableau pour les deux damiers et determine le nombre de case vide adjacente pour chaque case
int determine_taille_navire(Joueur defenseur); // retourne la taille du plus grand navire encore en etat
void mode_reperage_IA3(Joueur *attaquant,Joueur *defenseur,int *x,int *y); // choisis comme tir la case avec le plus de case adjacente vide
bool verifie_tire_touche_navire(Joueur *attaquant, Joueur *defenseur,int x,int y);

#endif // GAME_H
