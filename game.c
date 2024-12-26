#include "game.h"

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
                if(niveau == 1)
                {
                    touche = tirer_IA1(joueur2, joueur1); // Joueur 2 tire sur Joueur 1
                }
                else if(niveau == 2)
                {
                    touche = tirer_IA2(joueur2, joueur1); // Joueur 2 tire sur Joueur 1
                }
                else if(niveau == 3)
                {
                    touche = tirer_IA3(joueur2, joueur1); // Joueur 2 tire sur Joueur 1
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
    sleep(2);
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
            retenter = mode_chasse_IA3(attaquant,&x,&y,&last_x,&last_y);
        }
        else
        {
            if (retenter == false)
            {
                mode_reperage_IA3(attaquant,defenseur,&x,&y);
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
    do
    {
        *y = rand() % 10;
        *x = rand() % 10;
    } while (!verifier_tir_utile(*x, *y, attaquant->grille_tirs)); 
}


void mode_reperage_IA3(Joueur *attaquant,Joueur *defenseur,int *x,int *y)
{   
    bool flotte_intacte = true;
    for (int indice = 0; indice < 5; indice++)
    {
        if (defenseur->navires[indice].etat == false)
        {
            //flotte_intacte = false;
            break;
        } 
    }
    if (flotte_intacte == true)
    {
    do
    {
        *y = rand() % 10;
        *x = rand() % 10;
        printf("tire %d %d %d %d/ ",*y,*x,(*x % 2),(*y % 2));
    } while ((!verifier_tir_utile(*x, *y, attaquant->grille_tirs)) || !(((*x % 2) == 0 && (*y % 2) != 0) || ((*x % 2) == 1 && (*y % 2) != 1))); 
    }
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