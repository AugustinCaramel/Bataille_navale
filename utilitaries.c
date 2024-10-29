#include "utilitaries.h"

bool verifier_commande(char select[3])
{
    if (strcmp(select, "Q") == 0)
    {
        return false; // Quitte le jeu
    }
    else if (strcmp(select, "T") == 0)
    {
        printf("Fin du programme.\n");
        exit(0); // Termine complètement le programme
    }
    else if (strcmp(select, "I") == 0)
    {
        instructions(select); // Affiche les instructions
        return true;
    }
    return true; // Si aucune commande spéciale n'est saisie, le jeu continue
}

bool instructions(char select[3])
{
    printf("\n---------------------------------\n");
    printf("       Instructions du jeu       \n");
    printf("---------------------------------\n");

    printf("Bienvenue dans le jeu de Bataille Navale.\n\n");
    printf("Vous avez 5 navires de tailles disparates :\n");
    printf(" - Porte-avions (5 cases)\n - Croiseur (4 cases)\n - Destroyer (3 cases)\n - Sous-marin (3 cases)\n - Torpilleur (2 cases)\n");
    printf("\nVotre mission est de couler les navires ennemis.\n");
    printf("Vous placerez vos navires sur une grille de 10x10 cases.\n");
    printf("Les directions sont N (Nord), S (Sud), O (Ouest) et E (Est).\n");
    printf("\nCommandes globales, que vous pouvez utiliser tout au long de la partie :\n");
    printf(" - Q : Quitter les instructions, le menu principal ou la partie en cours\n");
    printf(" - T : Terminer le programme\n");
    printf(" - I : Revoir les instructions\n");
    printf("---------------------------------\n");
    do
    {
        printf("Entrez Q pour fermer les instructions : "); // Revient au menu principal ou à la partie en cours
        scanf("%s", select);
        if (!verifier_commande(select))
            return false;
    } while (strcmp(select, "Q") != 0);
    return true; // Continuer le jeu si une autre commande est entrée
}
int est_coordonnees(char select[3])
{
    return strlen(select) == 3 && isdigit(select[0]) && select[1] == '-' && isdigit(select[2]);
}
void afficher_ligne_grille(char grille[10][10], int i)
{
    for (int j = 0; j < 10; j++)
    {
        // Choix des couleurs selon le contenu de la case
        if (grille[i][j] == 'N')
            printf(BLEU " N " RESET); // Navire
        else if (grille[i][j] == 'X')
            printf(ROUGE " X " RESET); // Tir réussi
        else if (grille[i][j] == 'O')
            printf(VERT " O " RESET); // Tir manqué
        else
            printf(" . "); // Case vide
    }
}

void afficher_grille(char grille[10][10])
{
    printf("\n  ");
    for (int i = 0; i < 10; i++)
    {
        printf(" %d ", i);
    }
    printf("\n");

    for (int i = 0; i < 10; i++)
    {
        printf("%d ", i);
        afficher_ligne_grille(grille, i);
        printf("\n");
    }
}

void remplir_grille(char grille[10][10])
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            grille[i][j] = '.';
        }
    }
}

bool position_valide(int pos_y, int pos_x, char orientation, int longueur, char grille[10][10])
{
    if (orientation == 'N' && pos_y >= longueur - 1 && pos_y <= 9 && pos_x >= 0 && pos_x <= 9)
    {
        for (int j = 0; j < longueur; j++)
        {
            if (grille[pos_y - j][pos_x] == 'N')
                return false;
        }
        return true;
    }
    else if (orientation == 'E' && pos_y >= 0 && pos_y <= 9 && pos_x >= 0 && pos_x <= 9 - longueur + 1)
    {
        for (int j = 0; j < longueur; j++)
        {
            if (grille[pos_y][pos_x + j] == 'N')
                return false;
        }
        return true;
    }
    else if (orientation == 'S' && pos_y >= 0 && pos_y <= 9 - longueur + 1 && pos_x >= 0 && pos_x <= 9)
    {
        for (int j = 0; j < longueur; j++)
        {
            if (grille[pos_y + j][pos_x] == 'N')
                return false;
        }
        return true;
    }
    else if (orientation == 'O' && pos_y >= 0 && pos_y <= 9 && pos_x >= longueur - 1 && pos_x <= 9)
    {
        for (int j = 0; j < longueur; j++)
        {
            if (grille[pos_y][pos_x - j] == 'N')
                return false;
        }
        return true;
    }
    else
        return false;
}



