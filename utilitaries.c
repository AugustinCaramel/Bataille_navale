#include "utilitaries.h"

bool verifier_commande()
{
    if (strcmp(saisie, "Q") == 0)
    {
        return false; // Retour en arrière, quitter
    }
    else if (strcmp(saisie, "T") == 0)
    {
        printf("Fin du programme.\n");
        exit(0); // Termine complètement le programme
    }
    else if (strcmp(saisie, "I") == 0)
    {
        afficher_instructions(); // Affiche les instructions
        return true;
    }
    return true; // Si aucune commande spéciale n'est saisie, le jeu continue
}


bool afficher_instructions()
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
        scanf("%s", saisie);
        if (!verifier_commande())
            return false;
    } while (strcmp(saisie, "Q") != 0);
    return true; // Continuer le jeu si une autre commande est entrée
}


int verifier_format_coordonnees()
{
    return strlen(saisie) == 3 && isdigit(saisie[0]) && saisie[1] == '-' && isdigit(saisie[2]);
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


bool verifier_position_valide(int pos_y, int pos_x, char orientation, int longueur, char grille[10][10])
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


void affichage_placements(bool IA, int indice, char type[15], int longueur, char nom[30])
{
    if (IA && indice == 1)
    {
        printf("\nPlacement du %s (%d cases).\n", type, longueur);
    }
    else if (!IA)
    {
        printf("\nPlacement du %s (%d cases) pour %s.\n", type, longueur, nom);
    }
}


void demander_coordonnees(int *pos_x, int *pos_y)
{
    do
    {
        printf("\nEntrez la position Y-X : ");
        scanf("%s", saisie); // On utilise saisie pour accueillir toutes les saisies de l'utilisateur, afin de mettre en place des commandes universelles
        if (!verifier_commande())
            return;
    } while (!verifier_format_coordonnees());
    sscanf(saisie, "%d-%d", pos_y, pos_x);
}


void demander_orientation(char *orientation)
{
    do
    {
        printf("Entrez l'orientation (N, S, O, E) : ");
        scanf("%s", saisie);
        if (!verifier_commande())
            return;
    } while (strcmp(saisie, "N") != 0 && strcmp(saisie, "S") != 0 && strcmp(saisie, "O") != 0 && strcmp(saisie, "E") != 0);
    sscanf(saisie, "%c", orientation);
}


void afficher_noms_joueurs(int att_indice, char att_nom[30], char def_nom[30])
{
    char nomA[50], nomB[50];   // Augmenter la taille pour tenir compte de la concaténation
    char tempA[50], tempB[50]; // Chaînes temporaires pour la concaténation
    
    strcpy(nomA, (att_indice == 1) ? att_nom : def_nom);
    strcpy(nomB, (att_indice == 1) ? def_nom : att_nom);

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


void initialiser_nom(char nom[30], bool IA, int indice)
{
    if (!IA || indice == 1)
    {
        printf("Entrez votre nom : ");
        scanf("%s", saisie);
        if (!verifier_commande())
        {
            return ;
        }
        strcpy(nom, saisie);
    }
    else
    {
        strcpy(nom, "l'ordinateur");
    }
}


bool verifier_tir_utile(int x, int y, char grille_tirs[10][10])
{
    if (grille_tirs[y][x]=='.')
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool trouver_tir_IA2(int *x, int *y, int *last_x, int *last_y, char grille_tirs[10][10])
{
    bool trouver = false;
    for (int ligne_y = 0; ligne_y <= *last_y; ligne_y++)
    {
        for (int colonne_x = 0; colonne_x <= 10; colonne_x++)
        {
            if ((grille_tirs[ligne_y][colonne_x] == 'X') && !((ligne_y >= *last_y) && (colonne_x >= *last_x)))
            {
                *x = colonne_x;
                *y = ligne_y;
                trouver = true;
            }
        }
        
    }
    if (trouver == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}


void melanger_liste(int ordre_tir[4])
{
    int depart,numero;

    depart = rand() % 4;

    for (int i = 0; i < 4; i++)
    {
        if (depart+i > 3)
        {
            depart = depart-4;
        }
        numero=depart+i;
        ordre_tir[numero] = i;
    }
}


void detecte_direction_navire(char grille_tirs[10][10],int x_tir,int y_tir,int *direction)
{
    int yplus=y_tir+1, ymoins=y_tir-1, xplus=x_tir+1, xmoins=x_tir-1;
    printf("coordonne direction:%d %d %d %d %d %d /",y_tir,yplus,ymoins,x_tir,xplus,xmoins);
    if ((grille_tirs[y_tir][xmoins]=='X'||grille_tirs[y_tir][xplus]=='X') && xmoins>=0 && xplus<10)
    {
        *direction = 1; // E ou W
    }
    else if ((grille_tirs[ymoins][x_tir]=='X'||grille_tirs[yplus][x_tir]=='X') && ymoins>=0 && yplus<10)
    {
        *direction = 2; // N ou S
    }
    else
    {
        *direction = 0; // toute direction
    }
    printf("direction:%d /",*direction);
}


void trouver_coordonnee_tir_IA(int ordre_tir[4],int numero_tir,int direction,int *x_tir,int *y_tir,int x,int y)
{
    if (ordre_tir[numero_tir] == 0 && direction !=1)     // N
    {
        *x_tir = x;
        *y_tir = y-1;
    }
    else if (ordre_tir[numero_tir] == 1 && direction !=2)     // E
    {
        *x_tir = x+1;
        *y_tir = y;
    }
    else if (ordre_tir[numero_tir] == 2 && direction !=1)     // S
    {
        *x_tir = x;
        *y_tir = y+1;
    }
    else if (ordre_tir[numero_tir] == 3 && direction !=2)     // W
    {
        *x_tir = x-1;
        *y_tir = y;
    }
    else
    {
        *x_tir = 11;
        *y_tir = 11;
    }
}


bool selectionner_tir_IA2(int *x, int *y, char grille_tirs[10][10])
{
    int y_tir,x_tir,ordre_tir[4] = {0,1,2,3};
    melanger_liste(ordre_tir);
    for (int numero_tir = 0; numero_tir < 4; numero_tir++)
    {
        trouver_coordonnee_tir_IA(ordre_tir,numero_tir,0,&x_tir,&y_tir,*x,*y);
        printf("/ %d %d /",x_tir,y_tir);
        if(verifier_tir_utile(x_tir, y_tir, grille_tirs) && x_tir >=0 && x_tir < 10 && y_tir >=0 && y_tir < 10)
        {
            *x = x_tir;
            *y = y_tir;
            printf("grille tire:%C ",grille_tirs[y_tir][x_tir]);
            return true;
        }
    }
    printf("error");
    return false;
}


bool selectionner_tir_IA3(int *x, int *y, char grille_tirs[10][10])
{
    int direction_tir,y_tir,x_tir,ordre_tir[4] = {0,1,2,3};
    melanger_liste(ordre_tir);
    detecte_direction_navire(grille_tirs,*x,*y,&direction_tir);
    for (int numero_tir = 0; numero_tir < 4; numero_tir++)
    {
        trouver_coordonnee_tir_IA(ordre_tir,numero_tir,direction_tir,&x_tir,&y_tir,*x,*y);
        printf("coordonne tir:%d %d /",x_tir,y_tir);
        printf("num tir:%d /",numero_tir);
        if(verifier_tir_utile(x_tir, y_tir, grille_tirs) && x_tir >=0 && x_tir < 10 && y_tir >=0 && y_tir < 10)
        {
            *x = x_tir;
            *y = y_tir;
            printf("grille tire:%C ",grille_tirs[y_tir][x_tir]);
            return true;
        }
    }
    printf("error");
    return false;
}


void afficher_type_partie(bool IA, int niveau)
{
    if (IA)
        printf("\nPartie contre l'ordinateur : niveau %d\n",niveau);
    else
        printf("\nPartie classique : Joueur vs Joueur\n");
}