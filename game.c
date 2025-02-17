#include "game.h"

/* Fonctions de jeu classique */

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
    int compteur_tours = 0; // Compteur de tours (ne s'incrémente que s'il y a un changement de joueur)
    bool touche;            // Permet de savoir si un tir touche un navire
    bool rebelote = false;  // Permet de rejouer si un tir touche un navire

    while (true)
    {
        if (compteur_tours % 2 == 0) // Joueur 1 tire sur Joueur 2
        {
            if (!rebelote)
                printf((niveau != 0) ? "\n--- Votre tour ---\n" : "\n--- Tour de %s ---\n", joueur1->nom);
            touche = tirer(joueur1, joueur2);
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
                    touche = tirer_IA1(joueur2, joueur1);
                }
                else if (niveau == 2)
                {
                    touche = tirer_IA2(joueur2, joueur1, niveau);
                }
                else if (niveau == 3)
                {
                    touche = tirer_IA3(joueur2, joueur1, niveau);
                }
            }
            else
            {
                touche = tirer(joueur2, joueur1);
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

bool verifie_tire_touche_navire(Joueur *attaquant, Joueur *defenseur, int x, int y) // Vérifie si le tir touche un navire et lance la mise à jour des grilles
{
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

/* Fonctions utiles au mode de jeu Joueur versus Ordinateur */

Damier initialiser_damier(int num_damier) // Initialisation des attributs de damier
{
    Damier damier;
    damier.num_damier = num_damier;
    damier.nbr_tir = 50;
    damier.nbr_case = 0;
    damier.nbr_tir = 0;
    for (int num_case = 0; num_case < 50; num_case++)
    {
        damier.coordonee_case[num_case][0] = 0;
        damier.coordonee_case[num_case][1] = 0;
        damier.nbr_case_adjacente[num_case] = 0;
    }
}

void mode_chasse(Joueur *attaquant, int *x, int *y) // Choisit un tir aléatoire valide (utilisé en mode IA 1 et 2)
{
    do
    {
        *y = rand() % 10;
        *x = rand() % 10;
    } while (!verifier_tir_utile(*x, *y, attaquant->grille_tirs));
}

void mode_chasse_IA3(Joueur *attaquant, Joueur *defenseur, int *x, int *y) // Choisit un tir optimal en mode chasse pour le niveau 3 et le stocke dans x et y
{
    int liste_tir_0[50], liste_tir_1[50], nbr_case_max_0 = 0, nbr_case_max_1 = 0, taille_navire, choix_aleatoire;
    Damier damier_0 = initialiser_damier(0);
    Damier damier_1 = initialiser_damier(1);

    for (int i = 0; i < 5; i++)
    {
        if (defenseur->navires[i].etat == true)
        {
            taille_navire = defenseur->navires[i].longueur;
            break;
        }
    }

    genere_liste_case_vide(&damier_0, &damier_1, attaquant->grille_tirs, taille_navire);
    genere_liste_tir_optimal(&damier_0, &nbr_case_max_0);
    genere_liste_tir_optimal(&damier_1, &nbr_case_max_1);

    if (nbr_case_max_1 == nbr_case_max_0)
    {
        choisit_tir_optimal((rand() % 2) ? damier_1 : damier_0, x, y);
    }
    else
    {
        choisit_tir_optimal((nbr_case_max_1 > nbr_case_max_0) ? damier_1 : damier_0, x, y);
    }
}

bool mode_cible(Joueur *attaquant, int *x, int *y, int *last_x, int *last_y, int niveau) // Choisit un tir optimal en mode cible et le stocke dans x et y (en passant notamment par selectionner_tir)
{
    if (selectionner_tir(&*x, &*y, attaquant->grille_tirs, (niveau == 2) ? 2 : 3))
    {
        return false;
    }
    else
    {
        *last_x = *x;
        *last_y = *y;
        return true;
    }
}

void choisit_tir_optimal(Damier damier, int *x, int *y) // Choisit au hasard une des cases du damier avec le plus haut nombre de cases adjacentes vides et renvoie ces coordonnées
{
    int case_choisi, num_tir;

    num_tir = rand() % damier.nbr_tir;
    case_choisi = damier.liste_tir[num_tir];

    *x = damier.coordonee_case[case_choisi][1];
    *y = damier.coordonee_case[case_choisi][0];
}

void genere_liste_tir_optimal(Damier *damier, int *nbr_case_max) // Crée une liste avec le numéro de case de toutes les cases du damier ayant le nombre de cases adjacentes vides maximal
{
    int num_tir = 0;
    for (int num_case = 0; num_case < damier->nbr_case; num_case++)
    {
        if (damier->nbr_case_adjacente[num_case] > *nbr_case_max)
        {
            *nbr_case_max = damier->nbr_case_adjacente[num_case];
            for (; num_tir > 0; num_tir--)
            {
                damier->liste_tir[num_tir] = 0;
            }
            damier->liste_tir[num_tir] = num_case;
        }
        else if (damier->nbr_case_adjacente[num_case] == *nbr_case_max && damier->nbr_case_adjacente[num_case] != 0)
        {
            num_tir = num_tir + 1;
            damier->liste_tir[num_tir] = num_case;
        }
    }
    damier->nbr_tir = num_tir + 1;
}

void genere_probabilite_case(char grille_tirs[10][10], int *nbr_case_adjacente, int ligne, int colone, int taille_navire) // Détermine le nombre de cases vides dans les 4 directions de la case sélectionnée
{
    int compteur = 0;
    if (grille_tirs[ligne][colone] == '.')
    {
        for (int ligne_decale = ligne; grille_tirs[ligne_decale][colone] == '.' && ligne_decale < (ligne + taille_navire) && ligne_decale <= 9 && compteur < ((4 * taille_navire) - 3); ligne_decale++)
        {
            compteur = compteur + 1;
        }
        for (int ligne_decale = (ligne - 1); grille_tirs[ligne_decale][colone] == '.' && ligne_decale > (ligne - taille_navire) && ligne_decale >= 0 && compteur < ((4 * taille_navire) - 3); ligne_decale--)
        {
            compteur = compteur + 1;
        }

        for (int colone_decale = (colone + 1); grille_tirs[ligne][colone_decale] == '.' && colone_decale < (colone + taille_navire) && colone_decale <= 9 && compteur < ((4 * taille_navire) - 3); colone_decale++)
        {
            compteur = compteur + 1;
        }
        for (int colone_decale = (colone - 1); grille_tirs[ligne][colone_decale] == '.' && colone_decale > (colone - taille_navire) && colone_decale >= 0 && compteur < ((4 * taille_navire) - 3); colone_decale--)
        {
            compteur = compteur + 1;
        }
    }
    *nbr_case_adjacente = compteur;
}

void genere_liste_case_vide(Damier *damier_0, Damier *damier_1, char grille_tirs[10][10], int taille_navire) // Crée des tableaux pour les deux damiers et détermine le nombre de cases vides adjacentes pour chaque case
{
    int num_case_0 = 0, num_case_1 = 0;
    for (int ligne = 0; ligne < 10; ligne++)
    {
        for (int colone = 0; colone < 10; colone++)
        {
            if ((ligne % 2 == 0 && colone % 2 == 0) || (ligne % 2 != 0 && colone % 2 != 0))
            {
                genere_probabilite_case(grille_tirs, &damier_0->nbr_case_adjacente[num_case_0], ligne, colone, taille_navire);
                damier_0->coordonee_case[num_case_0][0] = ligne;
                damier_0->coordonee_case[num_case_0][1] = colone;
                num_case_0 = num_case_0 + 1;
            }
            else
            {
                genere_probabilite_case(grille_tirs, &damier_1->nbr_case_adjacente[num_case_1], ligne, colone, taille_navire);
                damier_1->coordonee_case[num_case_1][0] = ligne;
                damier_1->coordonee_case[num_case_1][1] = colone;
                num_case_1 = num_case_1 + 1;
            }
        }
    }
    damier_0->nbr_case = num_case_0;
    damier_1->nbr_case = num_case_1;
}

/* Fonctions de tir différenciées par niveau */

bool tirer(Joueur *attaquant, Joueur *defenseur) // Fonction de tir pour les joueurs humains
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

bool tirer_IA1(Joueur *attaquant, Joueur *defenseur) // Mode de jeu IA 1 : Tir aléatoire pur
{
    ajoute_delai();

    int y, x;
    mode_chasse(attaquant, &x, &y); // Choisit un tir aléatoire jusqu'à ce qu'il soit utile (non déjà tiré)

    if (verifie_tire_touche_navire(attaquant, defenseur, x, y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool tirer_IA2(Joueur *attaquant, Joueur *defenseur, int niveau) // Mode de jeu IA 2 : Tir aléatoire et peut cibler un navire une fois détecté
{
    ajoute_delai();

    int y, x, last_y = 10, last_x = 10;
    bool retenter = false;

    do
    {
        if (trouver_tir_IA2(&x, &y, &last_x, &last_y, attaquant->grille_tirs)) // Retourne true si un X non entouré d'anciens tirs est trouvé
        {
            retenter = mode_cible(attaquant, &x, &y, &last_x, &last_y, niveau); // True si tous les tirs adjacents ont déjà été tirés, et on remonte au X précédent (un peu plus au Nord - Ouest)
        }
        else if (!retenter)
        {
            mode_chasse(attaquant, &x, &y); // Si aucun X n'est trouvé, on tire aléatoirement
        }
    } while (retenter);

    if (verifie_tire_touche_navire(attaquant, defenseur, x, y)) // Vérifie si le tir touche un navire
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool tirer_IA3(Joueur *attaquant, Joueur *defenseur, int niveau) // Mode de jeu IA 3 : Tir en damier sur la case avec le plus de cases adjacentes vides et traque un navire en interprétant sa direction
{
    ajoute_delai();
    int y, x, last_y = 10, last_x = 10;
    bool retenter = false;

    do
    {
        if (trouver_tir_IA2(&x, &y, &last_x, &last_y, attaquant->grille_tirs))
        {
            retenter = mode_cible(attaquant, &x, &y, &last_x, &last_y, niveau);
        }
        else
        {
            if (retenter == false)
            {
                mode_chasse_IA3(attaquant, defenseur, &x, &y);
            }
        }

    } while (retenter);

    // Vérifie si le tir touche un navire
    if (verifie_tire_touche_navire(attaquant, defenseur, x, y))
    {
        return true;
    }
    else
    {
        return false;
    }
}