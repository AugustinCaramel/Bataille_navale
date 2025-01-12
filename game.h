#ifndef GAME_H
#define GAME_H

#include "joueur.h"

extern char saisie[3]; // Variable globale pour les saisies

typedef struct // Deux damiers sont initialisés dans le mode de jeu IA 3, un qui correspond aux "cases blanches" et l'autre aux "cases noires"
{
    int num_damier;             // Pair pair = 0, pair impair = 1
    int nbr_case;               // Nombre de cases vides
    int nbr_tir;                // Nombre de cases ayant un tir optimal
    int coordonee_case[50][2];  // Liste de coordonnées de chaque case, 0 = x, 1 = y
    int nbr_case_adjacente[50]; // Liste avec le nombre de cases vides adjacentes de chaque case
    int liste_tir[50];          // Liste des numéros de cases des tirs optimaux
} Damier;

/* Fonctions de jeu classique */

void menu_principal(); // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
void choix_niveau(int *niveau); // Affiche les différents niveaux et modifie la valeur de niveau avec la valeur saisie par l'utilisateur
void lancer_partie(int niveau); // Initialise les deux joueurs et lance les tours
void lancer_tours(Joueur *joueur1, Joueur *joueur2, int niveau); // Gère les tours de jeu (la fonction n'est pas quittée tant qu'une partie est toujours en cours)
bool verifie_tire_touche_navire(Joueur *attaquant, Joueur *defenseur, int x, int y);

/* Fonctions utiles au mode de jeu Joueur versus Ordinateur */

Damier initialiser_damier(int num_damier); // Initialisation des attributs de damier
void mode_chasse(Joueur *attaquant, int *x, int *y); // Choisit un tir aléatoire valide
void mode_chasse_IA3(Joueur *attaquant, Joueur *defenseur, int *x, int *y); // Choisit comme tir la case avec le plus de cases adjacentes vides
bool mode_cible(Joueur *attaquant, int *x, int *y, int *last_x, int *last_y, int niveau); // Choisit un tir optimal en mode cible et le stocke dans x et y (en passant notamment par selectionner_tir)
void choisit_tir_optimal(Damier damier, int *x, int *y); // Choisit au hasard une des cases du damier avec le plus haut nombre de cases adjacentes vides et renvoie ces coordonnées
void genere_liste_tir_optimal(Damier *damier, int *nbr_case_max); // Crée une liste avec le numéro de case de toutes les cases du damier ayant le nombre de cases adjacentes vides maximal
void genere_probabilite_case(char grille_tirs[10][10], int *nbr_case_vide, int ligne, int colonne, int taille_navire); // Détermine le nombre de cases vides dans les 4 directions de la case sélectionnée
void genere_liste_case_vide(Damier *damier_0, Damier *damier_1, char grille_tirs[10][10], int taille_navire); // Crée des tableaux pour les deux damiers et détermine le nombre de cases vides adjacentes pour chaque case

/* Fonctions de tir différenciées par niveau */

bool tirer(Joueur *attaquant, Joueur *defenseur); // Fonction de tir pour les joueurs humains
bool tirer_IA1(Joueur *attaquant, Joueur *defenseur); // Mode de jeu IA 1 : Tir aléatoire
bool tirer_IA2(Joueur *attaquant, Joueur *defenseur, int niveau); // Mode de jeu IA 2 : Tir aléatoire et peut cibler un navire une fois détecté
bool tirer_IA3(Joueur *attaquant, Joueur *defenseur, int niveau); // Mode de jeu IA 3 : Tir en damier sur la case avec le plus de cases adjacentes vides et traque un navire en interprétant sa direction

#endif // GAME_H
