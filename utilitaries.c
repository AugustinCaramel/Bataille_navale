#include "utilitaries.h"

bool verifier_commande() // Termine le programme si la variable globale saisie = T, renvoie false si saisie = Q, affiche les instructions si saisie = I, et renvoie true si saisie != Q et != T
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

bool afficher_instructions() // Affiche les instructions
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

int verifier_format_coordonnees() // Vérifie que les coordonnées saisies sont bien de la forme "x-y"
{
    return strlen(saisie) == 3 && isdigit(saisie[0]) && saisie[1] == '-' && isdigit(saisie[2]);
}

void afficher_ligne_grille(char grille[10][10], int i) // Affiche une ligne de la grille
{
    for (int j = 0; j < 10; j++)
    {
        // Choix des couleurs selon le contenu de la case
        if (grille[i][j] == 'N')
            printf(" N "); // Navire
        else if (grille[i][j] == 'X')
            printf(" X "); // Tir réussi
        else if (grille[i][j] == 'O')
            printf(" O "); // Tir manqué
        else
            printf(" . "); // Case vide
    }
}

void afficher_grille(char grille[10][10]) // Affiche la grille
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

void remplir_grille(char grille[10][10]) // Remplit la grille de points (initialisation)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            grille[i][j] = '.';
        }
    }
}

bool verifier_position_valide(int pos_y, int pos_x, char orientation, int longueur, char grille[10][10]) // Vérifie que la position est valide lors du placement d'un navire (dans la grille et sans chevauchement)
{
    int dy = 0, dx = 0;
    switch (orientation)
    {
    case 'N':
        dy = -1;
        break;
    case 'E':
        dx = 1;
        break;
    case 'S':
        dy = 1;
        break;
    case 'O':
        dx = -1;
        break;
    default:
        return false;
    }

    for (int j = 0; j < longueur; j++)
    {
        int ny = pos_y + j * dy;
        int nx = pos_x + j * dx;
        if (ny < 0 || ny > 9 || nx < 0 || nx > 9 || grille[ny][nx] == 'N')
        {
            return false;
        }
    }
    return true;
}

void affichage_placements(int niveau, int indice, char type[15], int longueur, char nom[30]) // Affichage lors du placement des navires
{
    if (niveau != 0 && indice == 1)
    {
        if (longueur == 5)
            printf("\nPlacement du %s", type);
        else
            printf(", du %s", type);
        if (longueur == 2)
            printf(".\n");
    }
    else if (niveau == 0)
    {
        printf("\nPlacement du %s (%d cases) pour %s.\n", type, longueur, nom);
    }
}

void demander_coordonnees(int *pos_x, int *pos_y) // Demande des coordonnées au joueur (initialisation et lors des tirs)
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

void demander_orientation(char *orientation) // Demande l'orientation au joueur (initialisation des navires)
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

void afficher_noms_joueurs(int att_indice, char att_nom[30], char def_nom[30]) // Affiche les noms des joueurs sous l'affichage des grilles
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

void initialiser_nom(char nom[30], int niveau, int indice) // Demande le nom du joueur ou initialise le nom de l'IA : "l'ordinateur"
{
    if (niveau == 0 || indice == 1)
    {
        printf("Entrez votre nom : ");
        scanf("%s", saisie);
        if (!verifier_commande())
        {
            return;
        }
        strcpy(nom, saisie);
    }
    else
    {
        strcpy(nom, "l'ordinateur");
    }
}

bool verifier_tir_utile(int x, int y, char grille_tirs[10][10]) // Vérifie si le tir est utile (case non encore touchée)
{
    if (x < 0 || x > 9 || y < 0 || y > 9)
    {
        return false;
    }
    if (grille_tirs[y][x] == '.')
    {
        return true;
    }
    else
    {
        return false;
    }
}

void afficher_type_partie(int niveau) // Affiche le type de partie en cours (classique ou contre l'ordinateur)
{
    if (niveau != 0)
        printf("\nPartie contre l'ordinateur : niveau %d\n", niveau);
    else
        printf("\nPartie classique : Joueur vs Joueur\n");
}

void decaler_gauche_chasse(int tableau[][2], int indice, int *taille) // Supprime une case du tableau de chasse à l'indice donné
{
    for (int i = indice; i < *taille - 1; i++)
    {
        tableau[i][0] = tableau[i + 1][0];
        tableau[i][1] = tableau[i + 1][1];
    }
    *taille = *taille - 1;
}

void actualiser_chasse(int chasse[50][2], int *taille_chasse, int y, int x) // Actualise le tableau de chasse en retirant la case (y, x) si elle est présente
{
    int indice = 0;
    while (indice < *taille_chasse && (chasse[indice][0] != y || chasse[indice][1] != x))
    {
        indice++;
    }
    if (indice < *taille_chasse)
    {
        printf("Case retiree ; %d-%d\n", chasse[indice][0], chasse[indice][1]);
        decaler_gauche_chasse(chasse, indice, taille_chasse);
    }
}