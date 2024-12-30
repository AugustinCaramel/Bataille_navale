#include "joueur.h"

Joueur initialiser_joueur(int indice, int niveau) // Initialise les attributs du joueur : grilles, nom, placement des navires, indice (avec l'indice passé en paramètre)
{
    Joueur joueur;
    remplir_grille(joueur.grille); // Pas besoin de pointeur car la grille est un tableau
    remplir_grille(joueur.grille_tirs);
    joueur.indice = indice;

    if (niveau!=0 && indice == 1)
        printf("\n--- Initialisation du Joueur ---\n");
    else if (niveau==0)
        printf("\n--- Initialisation du Joueur %d ---\n", indice);

    initialiser_nom(joueur.nom, niveau, indice);
    if (strcmp(saisie, "Q") == 0)
        return joueur;

    placer_navires(&joueur, niveau); // Pointeur pour que les navires soient référencés dans les attributs du joueur
    return joueur;
}

Cible initialiser_cible() // Initialise les attributs de la cible : état, tableau des positions touchées (vide), orientations possibles, nombre de touches
{
    Cible cible;
    cible.etat = false;
    for (int i = 0; i < 20; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            cible.touche_position[i][j] = -1;
        }
    }
    for (int i = 0; i < 4; i++)
    {
        cible.orientation[i] = false;
    }
    cible.nombre_touches = 0;
    return cible;
}

void placer_navires(Joueur *joueur, int niveau) // Définit le mode aléatoire ou non, crée les 5 navires et les positionne sur la grille
{
    bool aleatoire = (niveau!=0 && joueur->indice == 2) ? true : false;

    if (!(niveau!=0 && joueur->indice == 2))
    {
        do
        {
            printf("\nVoulez-vous que l'ordinateur place vos navires ?\n");
            printf("O (Oui) ou N (Non) : ");
            scanf("%s", saisie);
            if (!verifier_commande())
            {
                return;
            }
        } while (!strcmp(saisie, "O") == 0 && !strcmp(saisie, "N") == 0);

        if (strcmp(saisie, "O") == 0)
        {
            aleatoire = true;
        }
    }

    for (int i = 0; i < 5; i++)
    {
        joueur->navires[i] = creer_navire(i, joueur->nom, joueur->grille, aleatoire, niveau, joueur->indice);
        if (strcmp(saisie, "Q") == 0)
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
        if (!aleatoire || i == 4 && (niveau==0 || niveau!=0 && joueur->indice == 1)) // déterminé avec tableau de Karnaugh !
            afficher_grille(joueur->grille);
    }
}

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

void update_grille_tirs(Joueur *defenseur, Joueur *attaquant, int indice) // Remplace les X par des N sur un navire, sur une grille de tirs, si le navire est coulé
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

bool verifier_etat_navire(Joueur *defenseur, int indice_navire, int indiceY, int indiceX) // Renvoie false si le navire est entièrement touché
{
    for (int j = 0; j < defenseur->navires[indice_navire].longueur; j++)
    {
        if (defenseur->grille[defenseur->navires[indice_navire].pos_y + j * indiceY][defenseur->navires[indice_navire].pos_x + j * indiceX] == 'N')
        {
            return true;
        }
    }
    return false;
}

bool update_navires(Joueur *attaquant, Joueur *defenseur, Cible *cible, int niveau) // Vérifie l'ensemble de la flotte du joueur, met à jour les grilles de tirs et annonce si un nouveau navire est coulé
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
                navire_ok = verifier_etat_navire(defenseur, i, 0, -1);
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
                if (niveau == 3 || niveau == 2)
                    actualiser_cible(cible, defenseur->navires[i]);
                return true;
            }
        }
    }
    afficher_grilles(attaquant, defenseur);
    return false;
}

bool verifier_joueur_a_perdu(Joueur *joueur) // Vérifie que le joueur a encore >=1 navire actif
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