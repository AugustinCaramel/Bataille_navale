#include "joueur.h"

void afficher_les_noms(Joueur *attaquant, Joueur *defenseur)
{
    char nomA[50], nomB[50];   // Augmenter la taille pour tenir compte de la concaténation
    char tempA[50], tempB[50]; // Chaînes temporaires pour la concaténation
    if (attaquant->indice == 1)
    {
        strcpy(nomA, attaquant->nom);
        strcpy(nomB, defenseur->nom);
    }
    else
    {
        strcpy(nomA, defenseur->nom);
        strcpy(nomB, attaquant->nom);
    }
    // Concaténer "Tirs de " avec les noms
    strcpy(tempA, "Tirs de ");
    strcat(tempA, nomA); // "Tirs de attaquant"

    strcpy(tempB, "Tirs de ");
    strcat(tempB, nomB); // "Tirs de defenseur"

    // Copier le résultat final dans nomA et nomB
    strcpy(nomA, tempA);
    strcpy(nomB, tempB);

    int largeur = 35; // Largeur d'une grille avec espacement
    int longueur_nomA = strlen(nomA);
    int longueur_nomB = strlen(nomB);

    int espacesAvantNomA = (largeur - longueur_nomA) / 2;
    int espacesAvantNomB = (largeur - longueur_nomB) / 2;

    printf("\n%*s%s%*s", espacesAvantNomA, "", nomA, espacesAvantNomA, ""); // Centrer le nomA
    printf("%*s%s\n", espacesAvantNomB, "", nomB);                          // Centrer le nomB
}

void afficher_grilles(Joueur *attaquant, Joueur *defenseur) // Affichage des grilles de tirs des deux joueurs côte-côte
{
    char(*grilleA)[10], (*grilleB)[10]; // Pour considérer Grilles A et B comme des pointeurs.

    if (attaquant->indice == 1)
    {
        grilleA = attaquant->grille_tirs;
        grilleB = defenseur->grille_tirs;
    }
    else
    {
        grilleA = defenseur->grille_tirs;
        grilleB = attaquant->grille_tirs;
    }
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
    afficher_les_noms(attaquant, defenseur);
}

void maj_grille_tirs(Joueur *defenseur, Joueur *attaquant, int indice)
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

bool etat_navire(Joueur *defenseur, int indice_navire, int indiceY, int indiceX)
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
                navire_ok = etat_navire(defenseur, i, -1, 0);
            }
            else if (defenseur->navires[i].orientation == 'S')
            {
                navire_ok = etat_navire(defenseur, i, 1, 0);
            }
            else if (defenseur->navires[i].orientation == 'O')
            {
                navire_ok = etat_navire(defenseur, i, 1, -1);
            }
            else if (defenseur->navires[i].orientation == 'E')
            {
                navire_ok = etat_navire(defenseur, i, 0, 1);
            }
            if (!navire_ok)
            {
                defenseur->navires[i].etat = false;
                maj_grille_tirs(defenseur, attaquant, i);
                afficher_grilles(attaquant, defenseur);
                printf("\nLe %s de %s a atteint le fond.\n", defenseur->navires[i].type, defenseur->nom);
                return;
            }
            else if (i == 4)
                afficher_grilles(attaquant, defenseur);
        }
    }
}

bool joueur_a_perdu(Joueur *joueur)
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

void placer_navires(Joueur *joueur, char select[3])
{
    bool aleatoire = false;
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
    for (int i = 0; i < 5; i++)
    {
        joueur->navires[i] = creer_navire(i, joueur->nom, joueur->grille, select, aleatoire);
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
        if (!aleatoire || i == 4)
            afficher_grille(joueur->grille);
    }
}

Joueur initialiser_joueur(char select[3], int indice)
{
    Joueur joueur;
    remplir_grille(joueur.grille); // Pas besoin de pointeur car la grille est un tableau
    remplir_grille(joueur.grille_tirs);
    joueur.indice = indice;
    printf("\n--- Initialisation du Joueur %d ---\n", indice);
    do
    {
        printf("Entrez votre nom : ");
        scanf("%s", select);
        if (!verifier_commande(select))
        {
            return joueur;
        }
    } while (strcmp(select, "Q") == 0);

    strcpy(joueur.nom, select);      // Utilise strcpy pour copier le nom
    placer_navires(&joueur, select); // Pointeur pour que les navires soient référencés dans les attributs du joueur
    return joueur;
}

void placer_navires_IA(Joueur *joueur, char select[3])
{
    bool aleatoire = false;
    if (joueur->indice == 1)
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
    if (joueur->indice == 2)
        aleatoire = true;

    for (int i = 0; i < 5; i++)
    {
        joueur->navires[i] = creer_navire_IA(i, joueur->indice, joueur->grille, select, aleatoire);
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
        if (!aleatoire || (i == 4 && joueur->indice == 1))
            afficher_grille(joueur->grille);
    }
}

Joueur initialiser_joueur_IA(char select[3], int indice)
{
    Joueur joueur;
    joueur.indice = indice;
    remplir_grille(joueur.grille); // Pas besoin de pointeur car la grille est un tableau
    remplir_grille(joueur.grille_tirs);
    if (indice == 1)
    {
        printf("\n--- Initialisation du Joueur ---\n");
        strcpy(joueur.nom, "Joueur");
    }
    else
    {
        strcpy(joueur.nom, "l'ordinateur");
    }
    placer_navires_IA(&joueur, select); // Pointeur pour que les navires soient référencés dans les attributs du joueur
    return joueur;
}
