#include "navire.h"

const char types_navires[5][100] = {"Porte-avions", "Croiseur", "Destroyer", "Sous-marin", "Torpilleur"};
const int longueurs_navires[5] = {5, 4, 3, 3, 2};
const char directions[4] = {'N', 'S', 'O', 'E'};


Navire creer_navire(int indice_navire, char nom[30], char grille[10][10], bool aleatoire, bool IA, int indice)
{
    Navire navire;
    strcpy(navire.type, types_navires[indice_navire]);  // Initialisation du type de navire
    navire.longueur = longueurs_navires[indice_navire]; // Initialisation de la longueur du navire
    navire.etat = true;                                 // Le navire est en bon état à sa création (n'est pas coulé)

    affichage_placements(IA, indice, navire.type, navire.longueur, nom);

    do
    {
        if (aleatoire)
        {
            navire.pos_y = rand() % 10;
            navire.pos_x = rand() % 10;
            navire.orientation = directions[rand() % 4];
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

    if (IA && aleatoire && indice == 1 || !IA && aleatoire)
        sleep(1); // Pour la conhérence des interactions

    return navire;
}
