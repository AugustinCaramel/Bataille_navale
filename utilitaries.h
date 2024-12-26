#ifndef UTILITARIES_H
#define UTILITARIES_H

// Gestion des bibiliothèques

#include <stdio.h>
#include <math.h>
#include <stdbool.h> // Manipuler des booléens
#include <string.h>  // Manipuler les chaînes de caractères
#include <stdlib.h>  // Manipuler les fonctions propres à l'aléatoire
#include <unistd.h>  // Fonctions sleep() qui me permet de fluidifier les interractions avec l'ordinateur
#include <time.h>    // Pour intégrer les fonctions aléatoires essentiellement
#include <ctype.h>   // Pour isdigit()

// Gestion des couleurs

#define RESET "\x1b[0m"
#define BLEU "\x1b[34m"
#define ROUGE "\x1b[31m"
#define VERT "\x1b[32m"
#define JAUNE "\x1b[32m"

char saisie[3]; // Variable globale pour les saisies

// Fonctions de plus bas niveau : ne font appel à aucune autre fonction, ou alors une fonction du même fichier

bool verifier_commande(); // Termine le programme si saisie = T, renvoie false si saisie = Q, affiche les instructions si saisie = I, et renvoie true si saisie != Q et != T
bool afficher_instructions(); // Affiche les instructions
int verifier_format_coordonnees();
void afficher_ligne_grille(char grille[10][10], int i);
void afficher_grille(char grille[10][10]);
void remplir_grille(char grille[10][10]);
bool verifier_position_valide(int pos_y, int pos_x, char orientation, int longueur, char grille[10][10]);
void affichage_placements(bool IA, int indice, char type[15], int longueur, char nom[30]);
void demander_coordonnees(int *pos_x, int *pos_y);
void demander_orientation(char *orientation);
void afficher_noms_joueurs(int att_indice, char att_nom[30], char def_nom[30]);
void initialiser_nom(char nom[30], bool IA, int indice);
bool verifier_tir_utile(int x, int y, char grille_tirs[10][10]);
void melanger_liste(int ordre_tir[4]);
void trouver_coordonnee_tir_IA2(int ordre_tir[4],int numero_tir,int *x_tir,int *y_tir,int x,int y);
void detecte_direction_navire(char grille_tirs[10][10],int x_tir,int y_tir,int *direction);
void trouver_coordonnee_tir_IA3(int ordre_tir[4],int numero_tir,int direction,int *x_tir,int *y_tir,int x,int y);
bool selectionner_tir_IA2(int *x, int *y, char grille_tirs[10][10]);
bool selectionner_tir_IA3(int *x, int *y, char grille_tirs[10][10]);
bool trouver_tir_IA2(int *x, int *y, int*last_x, int*last_y, char grille_tirs[10][10]);
void afficher_type_partie(bool IA, int niveau);

#endif // UTILITARIES_H
