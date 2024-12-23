#include "navire.h"

const char types_navires[5][100] = {"Porte-avions", "Croiseur", "Destroyer", "Sous-marin", "Torpilleur"};
const int longueurs_navires[5] = {5, 4, 3, 3, 2};
const char directions[4] = {'N', 'S', 'O', 'E'};

Navire creer_navire(int indice_navire, char nom[30], char grille[10][10], bool aleatoire, int niveau, int indice) // Initialise les attributs, demande la position et l'orientation jusqu'à qu'elles soient valables, puis retourne le navire
{
    Navire navire;
    strcpy(navire.type, types_navires[indice_navire]);  // Initialisation du type de navire
    navire.longueur = longueurs_navires[indice_navire]; // Initialisation de la longueur du navire
    navire.etat = true;                                 // Le navire est en bon état à sa création (n'est pas coulé)

    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = rand()%100000000L; // 100ms

    affichage_placements(niveau, indice, navire.type, navire.longueur, nom);

    do
    {
        if (aleatoire)
        {
            navire.pos_y = rand() % 10;
            navire.pos_x = rand() % 10;
            navire.orientation = directions[rand() % 4];
            // Ajout d'un délai pour améliorer l'aléatoire
            nanosleep(&ts, NULL);
        }
        else
        {
            demander_coordonnees(&navire.pos_y, &navire.pos_x);
            if (strcmp(saisie, "Q") == 0)
                return navire;

            demander_orientation(&navire.orientation);
            if (strcmp(saisie, "Q") == 0)
                return navire;

            if (!verifier_position_valide(navire.pos_y, navire.pos_x, navire.orientation, navire.longueur, grille))
                printf("\nPosition et orientation invalides.\n");
        }
    } while (!verifier_position_valide(navire.pos_y, navire.pos_x, navire.orientation, navire.longueur, grille));

    if (niveau!=0 && aleatoire && indice == 1 || niveau==0 && aleatoire)
        ts.tv_nsec = 500000000L; // 500ms
        nanosleep(&ts, NULL);

    return navire;
}