#include "joueur.h"

void afficher_grilles(Joueur *attaquant, Joueur *defenseur) // Affichage des grilles de tirs des deux joueurs côte-côte
{
    char(*grilleA)[10], (*grilleB)[10]; // Pour considérer Grilles A et B comme des pointeurs.

    grilleA = (attaquant->indice == 1) ? attaquant->grille_tirs : defenseur->grille_tirs;
    grilleB = (attaquant->indice == 1) ? defenseur->grille_tirs : attaquant->grille_tirs;

    printf("\n  ");
    for (int i = 0; i < 20; i++)
    {
        printf(" %d ", i % 10);
        if (i == 9)
            printf("     ");
    }
    printf("\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", i);
        afficher_ligne_grille(grilleA, i);
        printf("   %d ", i);
        afficher_ligne_grille(grilleB, i);
        printf("\n");
    }
    afficher_noms_joueurs(attaquant->indice, attaquant->nom, defenseur->nom);
}

void update_grille_tirs(Joueur *defenseur, Joueur *attaquant, int indice)
{
    for (int i = 0; i < attaquant->navires[indice].longueur; i++)
    {
        if (defenseur->navires[indice].orientation == 'N')
            attaquant->grille_tirs[defenseur->navires[indice].pos_y - i][defenseur->navires[indice].pos_x] = 'N';
        else if (defenseur->navires[indice].orientation == 'E')
            attaquant->grille_tirs[defenseur->navires[indice].pos_y][defenseur->navires[indice].pos_x + i] = 'N';
        else if (defenseur->navires[indice].orientation == 'S')
            attaquant->grille_tirs[defenseur->navires[indice].pos_y + i][defenseur->navires[indice].pos_x] = 'N';
        else
            attaquant->grille_tirs[defenseur->navires[indice].pos_y][defenseur->navires[indice].pos_x - i] = 'N';
    }
}

bool verifier_etat_navire(Joueur *defenseur, int indice_navire, int indiceY, int indiceX)
{
    for (int j = 0; j < defenseur->navires[indice_navire].longueur; j++)
    {
        if (defenseur->grille[defenseur->navires[indice_navire].pos_y + j * indiceY][defenseur->navires[indice_navire].pos_x + j * indiceX] == 'N')
        {
            return true;
        }
    }
}

void update_navires(Joueur *attaquant, Joueur *defenseur)
{
    for (int i = 0; i < 5; i++)
    {
        if (defenseur->navires[i].etat)
        {
            bool navire_ok = false;
            if (defenseur->navires[i].orientation == 'N')
            {
                navire_ok = verifier_etat_navire(defenseur, i, -1, 0);
            }
            else if (defenseur->navires[i].orientation == 'S')
            {
                navire_ok = verifier_etat_navire(defenseur, i, 1, 0);
            }
            else if (defenseur->navires[i].orientation == 'O')
            {
                navire_ok = verifier_etat_navire(defenseur, i, 1, -1);
            }
            else if (defenseur->navires[i].orientation == 'E')
            {
                navire_ok = verifier_etat_navire(defenseur, i, 0, 1);
            }
            if (!navire_ok)
            {
                defenseur->navires[i].etat = false;
                update_grille_tirs(defenseur, attaquant, i);
                afficher_grilles(attaquant, defenseur);
                printf("\nLe %s de %s a atteint le fond.\n", defenseur->navires[i].type, defenseur->nom);
                return;
            }
            else if (i == 4)
                afficher_grilles(attaquant, defenseur);
        }
    }
}

bool verifier_joueur_a_perdu(Joueur *joueur)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (joueur->grille[i][j] == 'N') // Il reste des navires
                return false;
        }
    }
    return true; // Tous les navires sont coulés
}

void placer_navires(Joueur *joueur, char select[3], bool IA)
{
    bool aleatoire = (IA && joueur->indice == 2) ? true : false;
    if (!(IA && joueur->indice == 2))
    {
        do
        {
            printf("\nVoulez-vous que l'ordinateur place vos navires ?\n");
            printf("O (Oui) ou N (Non) : ");
            scanf("%s", select);
            if (!verifier_commande(select))
            {
                return;
            }
        } while (strcmp(select, "Q") == 0);

        if (strcmp(select, "O") == 0)
        {
            aleatoire = true;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        joueur->navires[i] = creer_navire(i, joueur->nom, joueur->grille, select, aleatoire, IA, joueur->indice);
        if (strcmp(select, "Q") == 0)
            return; // Si l'utilisateur veut quitter

        for (int j = 0; j < joueur->navires[i].longueur; j++)
        {
            if (joueur->navires[i].orientation == 'N')
                joueur->grille[joueur->navires[i].pos_y - j][joueur->navires[i].pos_x] = 'N';
            else if (joueur->navires[i].orientation == 'E')
                joueur->grille[joueur->navires[i].pos_y][joueur->navires[i].pos_x + j] = 'N';
            else if (joueur->navires[i].orientation == 'S')
                joueur->grille[joueur->navires[i].pos_y + j][joueur->navires[i].pos_x] = 'N';
            else
                joueur->grille[joueur->navires[i].pos_y][joueur->navires[i].pos_x - j] = 'N';
        }
        if (!aleatoire || i == 4 && (!IA || IA && joueur->indice == 1)) // déterminé avec tableau de Karnaugh, c'était sympa
            afficher_grille(joueur->grille);
    }
}

Joueur initialiser_joueur(char select[3], int indice, bool IA)
{
    Joueur joueur;
    remplir_grille(joueur.grille); // Pas besoin de pointeur car la grille est un tableau
    remplir_grille(joueur.grille_tirs);
    joueur.indice = indice;

    if (IA && indice == 1)
        printf("\n--- Initialisation du Joueur ---\n");
    else if (!IA)
        printf("\n--- Initialisation du Joueur %d ---\n", indice);

    initialiser_nom(joueur.nom, select, IA, indice);
    if (strcmp(select, "Q") == 0)
                return joueur;

    placer_navires(&joueur, select, IA); // Pointeur pour que les navires soient référencés dans les attributs du joueur
    return joueur;
}