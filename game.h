#ifndef GAME_H
#define GAME_H

#include "joueur.h"

extern char saisie[3]; // Variable globale pour les saisies

typedef struct
{
    char nom[20];    // nom zone
    int x,y,x1,y1,taille_cible[2]; // taille cible sur x et y
} zone;

zone initialiser_zone(char nom[30],int x,int y,int x1,int y1);
void menu_principal(); // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
void choix_niveau(int *niveau); // Affiche les différents niveaux et modifie la valeur de niveau, avec la valeur de saisie
void lancer_partie(bool IA, int niveau); // Initialise les deux joueurs et lance les tours
void lancer_tours(Joueur *joueur1, Joueur *joueur2, bool IA, int niveau);
bool verifie_tire_touche_navire(Joueur *attaquant, Joueur *defenseur,int x,int y);
void update_compteur(Navire navires,zone zone,char grille_tirs[10][10],int colone,int ligne,int *compteur,int *nombre_navire,bool ligne_premier);
void remplis_estime_navires(Navire navires,int flotte_estime[4],int nombre_navire);
void adapter_au_bord(Navire navires,int *x);
void definir_ligne_colone(Navire navires,zone zone,bool ligne_premier,int *a1,int *a2,int *b1,int *b2);
void estime_zone(Navire navires,zone zone,char grille_tirs[10][10],int flotte_estime[4],bool ligne_premier);
void estime_flotte_par_zone(Navire navires[5],zone zone,char grille_tirs[10][10],int flotte_estime_x[4],int flotte_estime_y[4]);
int estime_taile_navire_cible(int flotte_estime[4]);
int choix_zone_favorable(int flotte_estime_x[8][4],int flotte_estime_y[8][4]);
int choix_zone_et_navire(Navire navires[5],zone zone[8],char grille_tirs[10][10]);
bool mode_chasse_IA2(Joueur *attaquant,int *x,int *y,int *last_x,int *last_y);
bool mode_chasse_IA3(Joueur *attaquant,int *x,int *y,int *last_x,int *last_y);
void mode_reperage(Joueur *attaquant,int *x,int *y);
void mode_reperage_IA3(Joueur *attaquant,Joueur *defenseur,int *x,int *y);
bool tirer(Joueur *attaquant, Joueur *defenseur);
bool tirer_IA1(Joueur *attaquant, Joueur *defenseur);
bool tirer_IA2(Joueur *attaquant, Joueur *defenseur);
bool tirer_IA3(Joueur *attaquant, Joueur *defenseur);

#endif // GAME_H
