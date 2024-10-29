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

// Fonctions de plus bas niveau : ne font appel à aucune autre fonction, ou alors une fonction du même fichier

bool verifier_commande(char select[3]);
bool instructions(char select[3]);
int est_coordonnees(char select[3]);
void afficher_ligne_grille(char grille[10][10], int i);
void afficher_grille(char grille[10][10]);
void remplir_grille(char grille[10][10]);
bool position_valide(int pos_y, int pos_x, char orientation, int longueur, char grille[10][10]);



#endif // UTILITARIES_H


