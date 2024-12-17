#include "game.h"

Cible initialiser_cible()
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

void menu_principal()
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
        scanf("%s", saisie);

        int niveau = 0;

        // Vérifie si l'utilisateur entre une commande spéciale
        if (!verifier_commande())
            return; // Quitte le programme si "T" est entré
        if (strcmp(saisie, "P") == 0)
            lancer_partie(false, niveau);

        if (strcmp(saisie, "O") == 0)
        {
            choix_niveau(&niveau);
            if (strcmp(saisie, "Q") != 0)
                lancer_partie(true, niveau);
        }
    } while (strcmp(saisie, "T") != 0);
}

void choix_niveau(int *niveau)
{
    do
    {
        printf("\n  1. Niveau 1 : l'ordinateur joue en mode random");
        printf("\n  2. Niveau 2 : l'ordinateur utilise quelques techniques");
        printf("\n  3. Niveau 3 : l'ordinateur utilise des algorithmes plus intelligents");
        printf("\n  Q. Revenir au menu principal");
        printf("\n\nVotre choix : ");

        scanf("%s", saisie);
        if (!verifier_commande())
            return;

        if ((strcmp(saisie, "1") != 0 && strcmp(saisie, "2") != 0 && strcmp(saisie, "3") != 0))
            printf("Saisie incorrecte.\n");

    } while (strcmp(saisie, "1") != 0 && strcmp(saisie, "2") != 0 && strcmp(saisie, "3") != 0);
    sscanf(saisie, "%d", niveau);
}

void lancer_partie(bool IA, int niveau)
{
    afficher_type_partie(IA, niveau);
    Joueur joueur1 = initialiser_joueur(1, IA);
    if (strcmp(saisie, "Q") == 0)
        return; // Si joueur 1 quitte

    Joueur joueur2 = initialiser_joueur(2, IA);
    if (strcmp(saisie, "Q") == 0)
        return; // Si joueur 2 quitte
    lancer_tours(&joueur1, &joueur2, IA, niveau);
}

void lancer_tours(Joueur *joueur1, Joueur *joueur2, bool IA, int niveau)
{
    int compteur_tours = 0;
    bool touche;
    bool rebelote = false;
    Cible cible = initialiser_cible();
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
            touche = tirer(joueur1, joueur2); // Joueur 1 tire sur Joueur 2
            if (strcmp(saisie, "Q") == 0)
                return;
        }
        else
        {
            if (!rebelote)
                printf("\n--- Tour de %s ---\n", joueur2->nom);
            if (IA)
                if (niveau == 1)
                {
                    touche = tirer_IA1(joueur2, joueur1); // Joueur 2 tire sur Joueur 1
                }
                else if (niveau == 2)
                {
                    touche = tirer_IA2(joueur2, joueur1); // Joueur 2 tire sur Joueur 1
                }
                else if (niveau == 3)
                {
                    touche = tirer_IA3(joueur2, joueur1, &cible); // Joueur 2 tire sur Joueur 1
                }
                else
                {
                    touche = tirer(joueur2, joueur1); // Joueur 2 tire sur Joueur 1
                    if (strcmp(saisie, "Q") == 0)
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

bool tirer(Joueur *attaquant, Joueur *defenseur)
{
    int y, x;
    do
    {
        printf("\n%s, entrez la localisation de tir (Y-X) : ", attaquant->nom);
        scanf("%s", saisie);
        if (!verifier_commande())
            break;
        sscanf(saisie, "%d-%d", &y, &x);
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

bool tirer_IA1(Joueur *attaquant, Joueur *defenseur)
{
    sleep(1);
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

bool tirer_IA2(Joueur *attaquant, Joueur *defenseur)
{
    sleep(1);
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

bool update_navires_IA3(Joueur *attaquant, Joueur *defenseur, Cible *cible)
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
                actualiser_cible(cible, defenseur->navires[i]);
                return true;
            }
        }
    }
    afficher_grilles(attaquant, defenseur);
    return false;
}

void actualiser_cible(Cible *cible, Navire navire)
{
    int incry, incrx;
    if (navire.orientation == 'N')
    {
        incrx = 0;
        incry = -1;
    }
    else if (navire.orientation == 'S')
    {
        incrx = 0;
        incry = 1;
    }
    else if (navire.orientation == 'O')
    {
        incry = 0;
        incrx = -1;
    }
    else if (navire.orientation == 'E')
    {
        incry = 0;
        incrx = 1;
    }
    for (int i = 0; i < navire.longueur; i++)
    {
        for (int j = 0; j < cible->nombre_touches; j++)
        {
            if (cible->touche_position[j][0] == navire.pos_y + incry * i && cible->touche_position[j][1] == navire.pos_x + incrx * i)
            {
                decaler_gauche(cible, j);
            }
        }
    }
    if (cible->nombre_touches == 0)
    {
        cible->etat = false;
    };
}

void decaler_gauche(Cible *cible, int indice)
{
    for (int i = indice; i < cible->nombre_touches; i++)
    {
        cible->touche_position[i][0] = cible->touche_position[i + 1][0];
        cible->touche_position[i][1] = cible->touche_position[i + 1][1];
    }
    cible->nombre_touches--;
}

void decaler_droite(Cible *cible, int y, int x)
{
    for (int i = cible->nombre_touches - 1; i >= 0; i--)
    {
        cible->touche_position[i + 1][0] = cible->touche_position[i][0];
        cible->touche_position[i + 1][1] = cible->touche_position[i][1];
    }
    cible->touche_position[0][0] = y;
    cible->touche_position[0][1] = x;
}

int definir_orientation(Cible cible)
{
    int compteur_vertical = 0;
    int compteur_horizontal = 0;
    for (int i = 0; i < cible.nombre_touches - 1; i++)
    {
        compteur_vertical += abs(cible.touche_position[i][0] - cible.touche_position[i + 1][0]);
        compteur_horizontal += abs(cible.touche_position[i][1] - cible.touche_position[i + 1][1]);
    }
    if (compteur_vertical >= compteur_horizontal)
    {
        return 0;
    }
    return 2;
}

bool tirer_IA3(Joueur *attaquant, Joueur *defenseur, Cible *cible)
{
    int y, x;
    int orientation = definir_orientation(*cible);
    sleep(1);
    if (cible->etat)
    {
        int position = 0;
        if (orientation == 0)
        {
            do
            {
                if (orientation == 0)
                { // Nord
                    y = cible->touche_position[position][0] - 1;
                    x = cible->touche_position[position][1];
                }
                else if (orientation == 1)
                { // Sud
                    y = cible->touche_position[position][0] + 1;
                    x = cible->touche_position[0][1];
                }
                orientation++;
                if (orientation == 2)
                {
                    orientation = 0;
                    position++;
                }
            } while (!verifier_tir_utile(x, y, attaquant->grille_tirs) && position <= cible->nombre_touches - 1);
            if (position == cible->nombre_touches)
            {
                orientation = 2;
                position = 0;
            }
            if (!verifier_tir_utile(x, y, attaquant->grille_tirs))
            {
                do
                {
                    if (orientation == 2)
                    { // Ouest
                        y = cible->touche_position[position][0];
                        x = cible->touche_position[position][1] - 1;
                    }
                    else if (orientation == 3)
                    { // Est
                        y = cible->touche_position[position][0];
                        x = cible->touche_position[position][1] + 1;
                    }
                    orientation++;
                    if (orientation == 4)
                    {
                        orientation = 2;
                        position++;
                    }
                } while (!verifier_tir_utile(x, y, attaquant->grille_tirs) && position <= cible->nombre_touches - 1);
            }
        }
        else
        {
            do
            {
                if (orientation == 2)
                { // Ouest
                    y = cible->touche_position[position][0];
                    x = cible->touche_position[position][1] - 1;
                }
                else if (orientation == 3)
                { // Est
                    y = cible->touche_position[position][0];
                    x = cible->touche_position[position][1] + 1;
                }
                orientation++;
                if (orientation == 4)
                {
                    orientation = 2;
                    position++;
                }
            } while (!verifier_tir_utile(x, y, attaquant->grille_tirs) && position <= cible->nombre_touches - 1);
            if (position == cible->nombre_touches)
            {
                orientation = 2;
                position = 0;
            }
            if (!verifier_tir_utile(x, y, attaquant->grille_tirs))
            {
                do
                {
                    if (orientation == 0)
                    { // Nord
                        y = cible->touche_position[position][0] - 1;
                        x = cible->touche_position[position][1];
                    }
                    else if (orientation == 1)
                    { // Sud
                        y = cible->touche_position[position][0] + 1;
                        x = cible->touche_position[0][1];
                    }
                    orientation++;
                    if (orientation == 2)
                    {
                        orientation = 0;
                        position++;
                    }
                } while (!verifier_tir_utile(x, y, attaquant->grille_tirs) && position <= cible->nombre_touches - 1);
            }
        }
        if (defenseur->grille[y][x] == 'N')
        {
            printf("\nDans le mille !\n");
            attaquant->grille_tirs[y][x] = 'X'; // Marque un tir réussi
            defenseur->grille[y][x] = 'X';      // Marque le navire touché
            bool bateau_coule = update_navires_IA3(attaquant, defenseur, cible);
            if (!bateau_coule)
            {
                decaler_droite(cible, y, x);
                cible->nombre_touches += 1;
            }
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
    else
    {
        do
        {
            y = rand() % 10;
            if (y%2==0) x = 2*(rand() % 5);
            else x = 2*(rand() % 5) + 1;
        } while (!verifier_tir_utile(x, y, attaquant->grille_tirs)); // Vérifie que le tir n'a pas déjà été tenté

        // Vérifie si le tir touche un navire
        if (defenseur->grille[y][x] == 'N')
        {
            printf("\nDans le mille !\n");
            attaquant->grille_tirs[y][x] = 'X'; // Marque un tir réussi
            defenseur->grille[y][x] = 'X';      // Marque le navire touché
            update_navires_IA3(attaquant, defenseur, cible);
            cible->etat = true;
            cible->nombre_touches = 1;
            cible->touche_position[0][0] = y;
            cible->touche_position[0][1] = x;
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
}