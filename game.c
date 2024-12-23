#include "game.h"

void menu_principal() // Lance le menu principal, la fonction se lance en début d'exécution et se maintient tout au long de cette dernière
{
    do
    {
        printf("\n-- Menu Principal --\n");
        printf("I. Afficher les instructions\n");
        printf("P. Partie Joueur contre Joueur\n");
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
            lancer_partie(niveau);

        if (strcmp(saisie, "O") == 0)
        {
            choix_niveau(&niveau);
            if (strcmp(saisie, "Q") != 0)
                lancer_partie(niveau);
        }
    } while (strcmp(saisie, "T") != 0);
}

void choix_niveau(int *niveau) // Affiche les différents niveaux et modifie la valeur de niveau avec la valeur saisie par l'utilisateur
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

void lancer_partie(int niveau) // Initialise les deux joueurs et lance les tours
{
    afficher_type_partie(niveau);
    Joueur joueur1 = initialiser_joueur(1, niveau);
    if (strcmp(saisie, "Q") == 0)
        return; // Si joueur 1 quitte

    Joueur joueur2 = initialiser_joueur(2, niveau);
    if (strcmp(saisie, "Q") == 0)
        return; // Si joueur 2 quitte
    lancer_tours(&joueur1, &joueur2, niveau);
}

void lancer_tours(Joueur *joueur1, Joueur *joueur2, int niveau) // Gère les tours de jeu (la fonction n'est pas quittée tant qu'une partie est toujours en cours)
{
    int compteur_tours = 0;                // Compteur de tours (ne s'incrémente que s'il y a un changement de joueur)
    bool touche;                           // Permet de savoir si un tir touche un navire
    bool rebelote = false;                 // Permet de rejouer si un tir touche un navire
    Cible cible = initialiser_cible();     // Modes de jeu IA 2 et 3
    int chasse[50][2], taille_chasse = 50; // Mode de jeu IA 3

    if (niveau == 3) // Mode de jeu IA 3 : Initialisation du tableau de chasse
    {
        int index = 0;
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 10; j++)
            {
                if (i % 2 == j % 2)
                {
                    chasse[index][0] = i;
                    chasse[index][1] = j;
                    index++;
                }
            }
        }
        melangerAvecPriorite(chasse, taille_chasse);
    }

    while (true)
    {
        if (compteur_tours % 2 == 0) // Joueur 1 tire sur Joueur 2
        {
            if (!rebelote)
                printf((niveau != 0) ? "\n--- Votre tour ---\n" : "\n--- Tour de %s ---\n", joueur1->nom);
            touche = tirer(joueur1, joueur2, &cible, niveau);
            if (strcmp(saisie, "Q") == 0)
                return;
        }
        else // Joueur 2 tire sur Joueur 1
        {
            if (!rebelote)
                printf("\n--- Tour de %s ---\n", joueur2->nom);

            if (niveau != 0)
            {
                if (niveau == 1)
                {
                    touche = tirer_IA1(joueur2, joueur1, &cible, niveau);
                }
                else if (niveau == 2)
                {
                    touche = tirer_IA2(joueur2, joueur1, &cible, niveau);
                }
                else if (niveau == 3)
                {
                    touche = tirer_IA3(joueur2, joueur1, &cible, niveau, chasse, &taille_chasse); // Joueur 2 tire sur Joueur 1
                }
            }
            else
            {
                touche = tirer(joueur2, joueur1, &cible, niveau);
                if (strcmp(saisie, "Q") == 0)
                    return;
            }
        }

        if (!touche)
        {
            compteur_tours++;
            rebelote = false;
        }
        else
        {
            if (verifier_joueur_a_perdu(joueur2)) // Vérifie si le joueur 2 a perdu
            {
                printf("\n%s est vainqueur de cette partie !\n", joueur1->nom);
                break;
            }
            else if (verifier_joueur_a_perdu(joueur1)) // Vérifie si le joueur 1 a perdu
            {
                printf("\n%s est vainqueur de cette partie !\n", joueur2->nom);
                break;
            }
            rebelote = true;
        }
    }
}

/* Les fonctions ci-dessous sont utilisées pour le mode de jeu IA 3 */

int calculerPonderation(int y, int x) // Retourne une pondération aléatoire pour une case y-x "à chasser", les cases centrales ayant une pondération plus faible
{
    if (x < 2 || x > 7 || y < 2 || y > 7)
    {
        return rand() % 50; // Pondération plus élevée pour les bords
    }
    return rand() % 10; // Pondération plus faible pour le centre
}

void melangerAvecPriorite(int chasse[50][2], int taille_chasse) // Mélange les coordonnées de chasse aléatoirement en priorisant les cases centrales
{
    int pond[taille_chasse]; // Tableau des pondérations (taille 50)

    for (int i = 0; i < taille_chasse; i++) // Remplissage du tableau de pondérations
    {
        int y = chasse[i][0], x = chasse[i][1];
        pond[i] = calculerPonderation(y, x);
    }

    for (int i = 0; i < taille_chasse - 1; i++)
    {
        for (int j = i + 1; j < taille_chasse; j++)
        {
            if (pond[i] > pond[j])
            {
                // Échange des pondérations
                int tempPond = pond[i];
                pond[i] = pond[j];
                pond[j] = tempPond;

                // Échange des coordonnées
                int temp0 = chasse[i][0];
                int temp1 = chasse[i][1];
                chasse[i][0] = chasse[j][0];
                chasse[i][1] = chasse[j][1];
                chasse[j][0] = temp0;
                chasse[j][1] = temp1;
            }
        }
    }
}

void trouver_case_adjacente(int orientation, int *y, int *x, Cible *cible, Joueur *attaquant, int niveau) // Trouve une case à cibler en fonction des dernières positions touchées
{
    // La fonction commence par les directions verticales si orientation est à 0, et horizontales si orientation est à 2.

    int position = 0;
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}}; // Nord, Sud, Ouest, Est

    if (niveau == 3)
    {
        for (int i = 0; i < 2; i++)
        {
            do
            {
                *y = cible->touche_position[position][0] + directions[orientation][0];
                *x = cible->touche_position[position][1] + directions[orientation][1];
                if (verifier_tir_utile(*x, *y, attaquant->grille_tirs))
                {
                    return;
                }
                orientation = (orientation + 1) % 2 + (orientation / 2) * 2; // Switch entre Nord et Sud ou Est et Ouest
                if (orientation % 2 == 0)
                {
                    position++;
                }
            } while (position < cible->nombre_touches);
            position = 0;
            orientation = (orientation < 2) ? 2 : 0; // Switch entre les directions horizontales et verticales
        }
    }
    else if (niveau == 2)
    {
        do
        {
            *y = cible->touche_position[position][0] + directions[orientation][0];
            *x = cible->touche_position[position][1] + directions[orientation][1];
            if (verifier_tir_utile(*x, *y, attaquant->grille_tirs))
            {
                return;
            }
            orientation = (orientation + 1) % 4;
            if (orientation == 0)
            {
                position++;
            }
        } while (position < cible->nombre_touches);
    }
}

/* Fonctions de tir différenciées par niveau */

bool tirer(Joueur *attaquant, Joueur *defenseur, Cible *cible, int niveau) // Fonction de tir pour les joueurs humains
{
    int y, x;
    do
    {
        printf("\n%s, entrez la localisation de tir (Y-X) : ", attaquant->nom);
        scanf("%s", saisie);
        if (!verifier_commande())
            return false;
        sscanf(saisie, "%d-%d", &y, &x);
    } while (y < 0 || y > 9 || x < 0 || x > 9); // Vérifie que les coordonnées sont valides

    // Vérifie si le tir touche un navire
    if (defenseur->grille[y][x] == 'N')
    {
        printf("\nDans le mille !\n");
        attaquant->grille_tirs[y][x] = 'X'; // Marque un tir réussi
        defenseur->grille[y][x] = 'X';      // Marque le navire touché
        update_navires(attaquant, defenseur, cible, niveau);
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

bool tirer_IA1(Joueur *attaquant, Joueur *defenseur, Cible *cible, int niveau) // Mode de jeu IA 1 : Tir aléatoire
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
        update_navires(attaquant, defenseur, cible, niveau);
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

bool tirer_IA2(Joueur *attaquant, Joueur *defenseur, Cible *cible, int niveau) // Mode de jeu IA 2 : Tir aléatoire, mais avec ciblage des cases adjacentes si un navire est touché
{
    int y, x;
    int orientation = rand() % 2; // Orientation de la cible (0 : Nord-Sud, 1 : Est-Ouest)

    /* Choix de la position de tir */

    if (cible->etat) // Si on est en mode Cible (navire touché)
    {
        trouver_case_adjacente(orientation, &y, &x, cible, attaquant, niveau);
    }
    else
    {
        do
        {
            y = rand() % 10;
            x = rand() % 10;
        } while (!verifier_tir_utile(x, y, attaquant->grille_tirs)); // Vérifie que le tir n'a pas déjà été tenté
    }

    /* Tir et mise à jour des paramètres de grille et de ciblage */

    if (defenseur->grille[y][x] == 'N')
    {
        printf("\nDans le mille !\n");
        attaquant->grille_tirs[y][x] = 'X'; // Marque un tir réussi
        defenseur->grille[y][x] = 'X';      // Marque le navire touché
        if (cible->etat)
        {
            bool bateau_coule = update_navires(attaquant, defenseur, cible, niveau);
            if (!bateau_coule)
            {
                decaler_droite_cible(cible, y, x);
                cible->nombre_touches += 1;
            }
        }
        else
        {
            update_navires(attaquant, defenseur, cible, niveau);
            cible->etat = true;
            cible->nombre_touches = 1;
            cible->touche_position[0][0] = y;
            cible->touche_position[0][1] = x;
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

bool tirer_IA3(Joueur *attaquant, Joueur *defenseur, Cible *cible, int niveau, int chasse[50][2], int *taille_chasse) // Mode de jeu IA 3 : Stratégies de chasse et de cible avancées
{
    int y, x, indice_chasse;
    int orientation = definir_orientation(*cible);

    /* Choix de la position de tir */

    if (cible->etat) // Si on est en mode Cible (navire touché)
    {
        trouver_case_adjacente(orientation, &y, &x, cible, attaquant, niveau);
        actualiser_chasse(chasse, taille_chasse, y, x); // Retire la case du tableau de chasse puisqu'elle est visée, qu'importe le résultat du tir
    }
    else
    {
        do
        {
            // La coordonnée 0 du tableau de chasse est normalement toujours celle qui sera sélectionnée. Mais par gestion des erreurs, on laisse une boucle while qui permettre d'éviter des bugs éventuels.
            indice_chasse = 0;
            y = chasse[indice_chasse][0];
            x = chasse[indice_chasse][1];
        } while (!verifier_tir_utile(x, y, attaquant->grille_tirs));
        decaler_gauche_chasse(chasse, indice_chasse, taille_chasse); // Retire la case du tableau de chasse puisqu'elle est visée, qu'importe le résultat du tir
    }

    /* Tir et mise à jour des paramètres de grille et de ciblage */

    if (defenseur->grille[y][x] == 'N')
    {
        printf("\nDans le mille !\n");
        attaquant->grille_tirs[y][x] = 'X'; // Marque un tir réussi
        defenseur->grille[y][x] = 'X';      // Marque le navire touché
        if (cible->etat)
        {
            bool bateau_coule = update_navires(attaquant, defenseur, cible, niveau);
            if (!bateau_coule)
            {
                decaler_droite_cible(cible, y, x);
                cible->nombre_touches += 1;
            }
        }
        else
        {
            update_navires(attaquant, defenseur, cible, niveau);
            cible->etat = true;
            cible->nombre_touches = 1;
            cible->touche_position[0][0] = y;
            cible->touche_position[0][1] = x;
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
