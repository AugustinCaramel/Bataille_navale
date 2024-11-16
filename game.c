#include "game.h"

void menu_principal(char select[3])
{
    do
    {
        printf("\n-- Menu Principal --\n");
        printf("I. Afficher les instructions\n");
        printf("P. Lancer une nouvelle partie\n");
        printf("O. Jouer contre l'ordinateur\n");
        printf("Q. Quitter le menu principal\n");
        printf("----------------------\n");
        printf("Votre choix : ");
        scanf("%s", select);

        int niveau = 0;

        // Vérifie si l'utilisateur entre une commande spéciale
        if (!verifier_commande(select))
            return; // Quitte le programme si "T" est entré
        if (strcmp(select, "P") == 0)
            lancer_partie(select, false, niveau);

        if (strcmp(select, "O") == 0)
        {
            choix_niveau(select, &niveau);
            if (strcmp(select, "Q") != 0)
                lancer_partie(select, true, niveau);
        }
    } while (strcmp(select, "T") != 0);
}


void choix_niveau(char select[3], int *niveau)
{
    do
    {
        printf("\n  1. Niveau 1 : l'ordinateur joue en mode random");
        printf("\n  2. Niveau 2 : l'ordinateur utilise quelques techniques");
        printf("\n  3. Niveau 3 : l'ordinateur utilise des algorithmes plus intelligents");
        printf("\n  Q. Revenir au menu principal");
        printf("\n\nVotre choix : ");

        scanf("%s", select);
        if (!verifier_commande(select))
            return;

        if ((strcmp(select, "1") != 0 && strcmp(select, "2") != 0 && strcmp(select, "3") != 0))
            printf("Saisie incorrecte.\n");

    } while (strcmp(select, "1") != 0 && strcmp(select, "2") != 0 && strcmp(select, "3") != 0);
    sscanf(select, "%d", niveau);
}

void lancer_partie(char select[3], bool IA, int niveau)
{
    afficher_type_partie(IA, niveau);
    Joueur joueur1 = initialiser_joueur(select, 1, IA);
    if (strcmp(select, "Q") == 0)
        return; // Si joueur 1 quitte

    Joueur joueur2 = initialiser_joueur(select, 2, IA);
    if (strcmp(select, "Q") == 0)
        return; // Si joueur 2 quitte

    lancer_tours(&joueur1, &joueur2, select, IA, niveau);
}


void lancer_tours(Joueur *joueur1, Joueur *joueur2, char select[3], bool IA, int niveau)
{
    int compteur_tours = 0;
    bool touche;
    bool rebelote = false;
    while (true)
    {
        if (compteur_tours % 2 == 0)
        {
            if (!rebelote)
            {
                if (IA)
                    printf("\n--- Votre tour ---\n");
                else
                    printf("\n--- Tour de %s ---\n", joueur1->nom);
            }
            touche = tirer(joueur1, joueur2, select); // Joueur 1 tire sur Joueur 2
            if (strcmp(select, "Q") == 0)
                return;
        }
        else
        {
            if (!rebelote)
                printf("\n--- Tour de %s ---\n", joueur2->nom);
            if (IA)
                touche = tirer_1(joueur2, joueur1, select); // Joueur 2 tire sur Joueur 1
            else
            {
                touche = tirer(joueur2, joueur1, select); // Joueur 2 tire sur Joueur 1
                if (strcmp(select, "Q") == 0)
                    return;
            }
        }

        // Ajoute une fonction pour vérifier si un joueur a perdu
        if (verifier_joueur_a_perdu(joueur2))
        {
            printf("\n%s est vainqueur de cette partie !\n", joueur1->nom);
            break;
        }
        else if (verifier_joueur_a_perdu(joueur1))
        {
            printf("\n%s est vainqueur de cette partie !\n", joueur2->nom);
            break;
        }

        if (!touche)
        {
            compteur_tours++;
            rebelote = false;
        }
        else
        {
            rebelote = true;
        }
    }
}


bool tirer(Joueur *attaquant, Joueur *defenseur, char select[3])
{
    int y, x;
    do
    {
        printf("\n%s, entrez la localisation de tir (Y-X) : ", attaquant->nom);
        scanf("%s", select);
        if (!verifier_commande(select))
            break;
        sscanf(select, "%d-%d", &y, &x);
    } while (y < 0 || y > 9 || x < 0 || x > 9); // Vérifie que les coordonnées sont valides

    // Vérifie si le tir touche un navire
    if (defenseur->grille[y][x] == 'N')
    {
        printf("\nDans le mille !\n");
        attaquant->grille_tirs[y][x] = 'X'; // Marque un tir réussi
        defenseur->grille[y][x] = 'X';      // Marque le navire touché
        update_navires(attaquant, defenseur);
        return true;
    }
    else if (defenseur->grille[y][x] == 'X')
    {
        printf("\nDans le mille (bis...)\n");
        afficher_grilles(attaquant, defenseur);
        return false;
    }
    else
    {
        printf("\nDans l'eau...\n");
        attaquant->grille_tirs[y][x] = 'O'; // Marque un tir manqué
        afficher_grilles(attaquant, defenseur);
        return false;
    }
}


bool tirer_1(Joueur *attaquant, Joueur *defenseur, char select[3])
{
    sleep(2);
    int y, x;
    do
    {
        y = rand() % 10;
        x = rand() % 10;
    } while (!verifier_tir_utile(x, y, attaquant->grille_tirs)); // Vérifie que le tir n'a pas déjà été tenté

    // Vérifie si le tir touche un navire
    if (defenseur->grille[y][x] == 'N')
    {
        printf("\nDans le mille !\n");
        attaquant->grille_tirs[y][x] = 'X'; // Marque un tir réussi
        defenseur->grille[y][x] = 'X';      // Marque le navire touché
        update_navires(attaquant, defenseur);
        return true;
    }
    else
    {
        printf("\nDans l'eau...\n");
        attaquant->grille_tirs[y][x] = 'O'; // Marque un tir manqué
        afficher_grilles(attaquant, defenseur);
        return false;
    }
}