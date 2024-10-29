#include "navire.h"

Navire creer_navire(int indice_navire, char nom[30], char grille[10][10], char select[3], bool aleatoire)
{
    char types_navires[5][100] = {"Porte-avions", "Croiseur", "Destroyer", "Sous-marin", "Torpilleur"};
    int longueurs_navires[5] = {5, 4, 3, 3, 2};
    char directions[4] = {'N', 'S', 'O', 'E'};

    Navire navire;
    strcpy(navire.type, types_navires[indice_navire]);
    navire.longueur = longueurs_navires[indice_navire];
    navire.etat = true;                                                                   // Le navire est en bon état à sa création (n'est pas coulé)
    printf("\nPlacement du %s (%d cases) pour %s.\n", navire.type, navire.longueur, nom); // Affichage

    do
    {
        if (aleatoire)
        {
            navire.pos_y = rand() % 10;
            navire.pos_x = rand() % 10;
            navire.orientation = directions[indice_navire % 4];
        }
        else
        {
            do
            {
                printf("\nEntrez la position Y-X (Ligne-Colonne) : ");
                scanf("%s", select); // On utilise select pour accueillir toutes les saisies de l'utilisateur, afin de mettre en place des commandes universelles
                if (!verifier_commande(select))
                    return navire;
            } while (!est_coordonnees(select));
            sscanf(select, "%d-%d", &navire.pos_y, &navire.pos_x);

            do
            {
                printf("Entrez l'orientation (N, S, O, E) : ");
                scanf("%s", select);
                if (!verifier_commande(select))
                    return navire; // Vérifie commande après saisie
            } while (strcmp(select, "N") != 0 && strcmp(select, "S") != 0 && strcmp(select, "O") != 0 && strcmp(select, "E") != 0);
            sscanf(select, "%c", &navire.orientation);

            if (!position_valide(navire.pos_y, navire.pos_x, navire.orientation, navire.longueur, grille))
                printf("\nPosition et orientation invalides.\n");
        }
    } while (!position_valide(navire.pos_y, navire.pos_x, navire.orientation, navire.longueur, grille));

    if (aleatoire)
        sleep(1);

    return navire;
}

Navire creer_navire_IA(int indice_navire, int indice, char grille[10][10], char select[3], bool aleatoire)
{
    char types_navires[5][100] = {"Porte-avions", "Croiseur", "Destroyer", "Sous-marin", "Torpilleur"};
    int longueurs_navires[5] = {5, 4, 3, 3, 2};
    char directions[4] = {'N', 'S', 'O', 'E'};

    Navire navire;
    strcpy(navire.type, types_navires[indice_navire]);
    navire.longueur = longueurs_navires[indice_navire];
    navire.etat = true;
    if (indice == 1)
    {
        printf("\nPlacement du %s (%d cases).\n", navire.type, navire.longueur); // Affichage
    }

    do
    {
        if (aleatoire)
        {
            navire.pos_y = rand() % 10;
            navire.pos_x = rand() % 10;
            navire.orientation = directions[indice_navire % 4];
        }
        else
        {
            do
            {
                printf("\nEntrez la position Y-X : ");
                scanf("%s", select); // On utilise select pour accueillir toutes les saisies de l'utilisateur, afin de mettre en place des commandes universelles
                if (!verifier_commande(select))
                    return navire;
            } while (!est_coordonnees(select));
            sscanf(select, "%d-%d", &navire.pos_y, &navire.pos_x);

            do
            {
                printf("Entrez l'orientation (N, S, O, E) : ");
                scanf("%s", select);
                if (!verifier_commande(select))
                    return navire; // Vérifie commande après saisie
            } while (strcmp(select, "N") != 0 && strcmp(select, "S") != 0 && strcmp(select, "O") != 0 && strcmp(select, "E") != 0);
            sscanf(select, "%c", &navire.orientation);

            if (!position_valide(navire.pos_y, navire.pos_x, navire.orientation, navire.longueur, grille))
                printf("\nPosition et orientation invalides.\n");
        }
    } while (!position_valide(navire.pos_y, navire.pos_x, navire.orientation, navire.longueur, grille));

    if (aleatoire && indice == 1)
        sleep(1);

    return navire;
}
