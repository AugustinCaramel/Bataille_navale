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

char saisie[3]; // Variable globale pour les saisies

// Fonctions de plus bas niveau : ne font appel à aucune autre fonction, ou alors une fonction du même fichier

bool verifier_commande(); // Termine le programme si la variable globale saisie = T, renvoie false si saisie = Q, affiche les instructions si saisie = I, et renvoie true si saisie != Q et != T
bool afficher_instructions(); // Affiche les instructions
int verifier_format_coordonnees(); // Vérifie que les coordonnées saisies sont bien de la forme "x-y"
void afficher_ligne_grille(char grille[10][10], int i); // Affiche une ligne de la grille
void afficher_grille(char grille[10][10]); // Affiche la grille
void remplir_grille(char grille[10][10]); // Remplit la grille de points (initialisation)
bool verifier_position_valide(int pos_y, int pos_x, char orientation, int longueur, char grille[10][10]); // Vérifie que la position est valide lors du placement d'un navire (dans la grille et sans chevauchement)
void affichage_placements(int niveau, int indice, char type[15], int longueur, char nom[30]); // Affichage lors du placement des navires
void demander_coordonnees(int *pos_x, int *pos_y); // Demande des coordonnées au joueur (initialisation et lors des tirs)
void demander_orientation(char *orientation); // Demande l'orientation au joueur (initialisation des navires)
void afficher_noms_joueurs(int att_indice, char att_nom[30], char def_nom[30]); // Affiche les noms des joueurs sous l'affichage des grilles
void initialiser_nom(char nom[30], int niveau, int indice); // Demande le nom du joueur ou initialise le nom de l'IA : "l'ordinateur"
bool verifier_tir_utile(int x, int y, char grille_tirs[10][10]); // Vérifie si le tir est utile (case non encore touchée)
void afficher_type_partie(int niveau); // Affiche le type de partie (IA ou joueur contre joueur)

#endif // UTILITARIES_H
