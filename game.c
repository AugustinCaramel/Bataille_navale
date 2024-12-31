#include "game.h"

damier initialiser_damier(int num_damier)
{
    damier damier;
    damier.num_damier = num_damier;
    damier.nbr_tir = 50;
    for (int num_case = 0; num_case < 50; num_case++)
    {
        damier.coordonee_case[num_case][0] = 0;
        damier.coordonee_case[num_case][1] = 0;
        damier.nbr_case_vide[num_case][0] = 0;
        damier.nbr_case_vide[num_case][1] = 0;
    }
    
}

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
                    touche = tirer_IA2(joueur2, joueur1);
                }
                else if (niveau == 3)
                {
                    touche = tirer_IA3(joueur2, joueur1);
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

bool tirer_IA1(Joueur *attaquant, Joueur *defenseur, Cible *cible, int niveau) // Mode de jeu IA 1 : Tir aléatoire
{
    sleep(1);
    int y, x;
    mode_reperage(attaquant,&x,&y);

    // Vérifie si le tir touche un navire
    if(verifie_tire_touche_navire(attaquant,defenseur,x,y))
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool tirer_IA2(Joueur *attaquant, Joueur *defenseur)
{
    sleep(2);
    int y, x,last_y = 11,last_x = 11;
    bool retenter = false;

    do
    {
        if(trouver_tir_IA2(&x, &y,&last_x,&last_y, attaquant->grille_tirs))
        {
            retenter = mode_chasse_IA2(attaquant,&x,&y,&last_x,&last_y);
        }
        else
        {
            if (retenter == false)
            {
                mode_reperage(attaquant,&x,&y);
            }
            
        }

    } while (retenter);
    

    // Vérifie si le tir touche un navire
    if(verifie_tire_touche_navire(attaquant,defenseur,x,y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool tirer_IA3(Joueur *attaquant, Joueur *defenseur)
{
    //sleep(2);
    int y, x,last_y = 11,last_x = 11;
    bool retenter = false;

    do
    {
        if(trouver_tir_IA2(&x, &y,&last_x,&last_y, attaquant->grille_tirs))
        {
            printf("trouver/ ");
            retenter = mode_chasse_IA3(attaquant,&x,&y,&last_x,&last_y);
        }
        else
        {
            printf("pas trouver/ ");
            if (retenter == false)
            {
                printf("retenter false/ ");
                mode_reperage(attaquant,&x,&y);
            }
            
        }

    } while (retenter);

        // Vérifie si le tir touche un navire
    if(verifie_tire_touche_navire(attaquant,defenseur,x,y))
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool mode_chasse_IA2(Joueur *attaquant,int *x,int *y,int *last_x,int *last_y)
{
    if (selectionner_tir_IA2(&*x, &*y,attaquant->grille_tirs))
    {
        printf("x:%d y:%d",*x,*y);
        return false;
    }
    else
    {
        *last_x = *x;
        *last_y = *y;
        return true;
    }
}


bool mode_chasse_IA3(Joueur *attaquant,int *x,int *y,int *last_x,int *last_y)
{
    if (selectionner_tir_IA3(&*x, &*y,attaquant->grille_tirs))
    {
        printf("x:%d y:%d",*x,*y);
        return false;
    }
    else
    {
        *last_x = *x;
        *last_y = *y;
        return true;
    }
}


void mode_reperage(Joueur *attaquant,int *x,int *y)
{
    printf("mode reperage/ ");
    do
    {
        *y = rand() % 10;
        *x = rand() % 10;
    } while (!verifier_tir_utile(*x, *y, attaquant->grille_tirs)); 
}


void mode_reperage_IA3(Joueur *attaquant,int *x,int *y)
{
    damier damier_0 = initialiser_damier(0);
    damier damier_1 = initialiser_damier(1);

    genere_liste_case_vide(&damier_0,&damier_1,attaquant->grille_tirs);

    printf("mode reperage/ ");
    do
    {
        *y = rand() % 10;
        *x = rand() % 10;
    } while (!verifier_tir_utile(*x, *y, attaquant->grille_tirs)); 
}




void genere_liste_case_vide(damier *damier_0,damier *damier_1,char grille_tirs[10][10])
{
    int num_case_0 = 0,num_case_1 = 0;
    for (int ligne = 0; ligne < 10; ligne++)
    {
        for (int colone = 0; colone < 10; colone++)
        {
            if ((ligne % 2 == 0 && colone % 2 == 0) || (ligne % 2 != 0 && colone % 2 != 0))
            {
            genere_probabilite_case(grille_tirs,damier_0->nbr_case_vide[num_case_0],ligne,colone);
            damier_0->coordonee_case[num_case_0][0] = ligne;
            damier_0->coordonee_case[num_case_0][1] = colone;
            num_case_0 = num_case_0 + 1;
            }
            else
            {
            genere_probabilite_case(grille_tirs,damier_1->nbr_case_vide[num_case_1],ligne,colone);
            damier_1->coordonee_case[num_case_1][0] = ligne;
            damier_1->coordonee_case[num_case_1][1] = colone;
            num_case_1 = num_case_1 + 1;
            }
        }
        
    }
    damier_0->nbr_tir = num_case_0 - 1;
    damier_1->nbr_tir = num_case_1 - 1;
    
}


void genere_probabilite_case(char grille_tirs[10][10],int nbr_case_vide[2],int ligne,int colone)
{
    int compteur = 0;
    for (int ligne_decale = ligne; grille_tirs[colone][ligne_decale] == '.' && compteur < 9; ligne_decale++)
    {
        compteur = compteur + 1;
    }
        for (int ligne_decale = (ligne - 1); grille_tirs[colone][ligne_decale] == '.' && compteur < 9; ligne_decale--)
    {
        compteur = compteur + 1;
    }
    nbr_case_vide[0] = compteur;
    compteur = 0;

        for (int colone_decale = colone; grille_tirs[colone_decale][ligne] == '.' && compteur < 9; colone_decale++)
    {
        compteur = compteur + 1;
    }
        for (int colone_decale = (colone - 1); grille_tirs[colone_decale][ligne] == '.' && compteur < 9; colone_decale--)
    {
        compteur = compteur + 1;
    }    
    nbr_case_vide[1] = compteur;
}


bool verifie_tire_touche_navire(Joueur *attaquant, Joueur *defenseur,int x,int y)
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