#ifndef UTILITARIES_H
#define UTILITARIES_H

/* Le fichier utilitaries définit les fonctions de plus bas niveau : celles qui ne font appel à aucune autre fonction, ou alors à une fonction définie dans le même fichier. */

// Import des bibiliothèques

#include <stdio.h>
#include <math.h>
#include <stdbool.h> // Manipuler des booléens
#include <string.h>  // Manipuler les chaînes de caractères
#include <stdlib.h>  // Manipuler les fonctions propres à l'aléatoire
#include <unistd.h>  // Fonctions sleep() qui me permet de fluidifier les interractions avec l'ordinateur
#include <time.h>    // Pour intégrer les fonctions aléatoires essentiellement
#include <ctype.h>   // Pour isdigit()

char saisie[3]; // Variable globale qui récupère toutes les saisies utilisateur (sauf les noms des joueurs, où la maniupulation est un peu différente)

/* Fonctions de vérification */

bool verifier_commande(); // Termine le programme si la variable globale saisie = T, renvoie false si saisie = Q, affiche les instructions si saisie = I, et renvoie true si saisie != Q et != T
bool verifier_format_coordonnees(); // Vérifie que les coordonnées saisies (stockées dans la variable globale saisie) sont bien de la forme "y-x"
bool verifier_position_valide(int pos_y, int pos_x, char orientation, int longueur, char grille[10][10]); // Vérifie que la position y-x et l'orientation passée en paramètre sont valides lors du placement d'un navire
bool verifier_tir_utile(int x, int y, char grille_tirs[10][10]); // Vérifie si le tir y-x est utile (case non encore touchée) lors des tirs de l'ordinateur

/* Fonctions d'affichage */

bool afficher_instructions(); // Affiche les instructions du jeu
void afficher_ligne_grille(char grille[10][10], int i); // Affiche la ligne d'indice i de la grille 10*10 passée en paramètre
void afficher_grille(char grille[10][10]); // Affiche la grille passée en paramètre
void affichage_placements(int niveau, int indice, char type[15], int longueur, char nom[30], bool aleatoire); // Affiche les textes introduisant le placement d'un navire selon le mode de jeu (mode IA ou non)
void afficher_noms_joueurs(int att_indice, char att_nom[30], char def_nom[30]); // Affiche les noms des joueurs : cette fonction est utilisée lors de l'affichage des grilles
void afficher_type_partie(int niveau); // Affiche le type de partie sélectionné (classique ou contre l'ordinateur)

/* Fonctions de saisie utilisateur */

void demander_coordonnees(int *pos_y, int *pos_x); // Demande des coordonnées au joueur et les stocke dans pos_x et pos_y si elles sont valides
void demander_orientation(char *orientation); // Demande une orientation au joueur et la stocke dans orientation si elle est valide (N, S, O ou E)
void initialiser_nom(char nom[30], int niveau, int indice); // Demande le nom du joueur ou initialise le nom du joueur 2 en "L'ordinateur" pour le mode IA

/* Fonctions opérationnelles */

void ajoute_delai(); // Ajoute un délai de 500ms
void remplir_grille(char grille[10][10]); // Remplit la grille passée en paramètre de points (utilisée lors de l'initialisation des grilles)
void melanger_liste(int ordre_tir[4]); // Mélange les directions possibles pour les tirs pour mettre en place un ordre aléatoire
bool trouver_tir_IA2(int *x, int *y, int*last_x, int*last_y, char grille_tirs[10][10]); // Trouve tir le plus au Sud-Est non encore complètement entouré de tirs passés et met ses coordonnées dans x et y
void trouver_coordonnee_tir_IA(int ordre_tir[4],int numero_tir,int direction,int *x_tir,int *y_tir,int x,int y); // Détermine les coordonnées d'un tir à proximité en fonction des paramètres passés
void detecte_direction_navire(char grille_tirs[10][10],int x_tir,int y_tir,int *direction); // Détermine la direction du navire en fonction des tirs déjà effectués
bool navire_imcomplet_sur_direction(char grille_tirs[10][10],int x,int y,int direction); // Vérifie si un navire est incomplet sur une direction donnée
bool selectionner_tir(int *x, int *y, char grille_tirs[10][10], int niveau); // À partir d'une position, teste les positions adjacentes de manière aléatoire jusqu'à trouver un tir

#endif // UTILITARIES_H
