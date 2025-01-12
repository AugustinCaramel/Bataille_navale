#include "utilitaries.h"

/* Fonctions de vérification */

bool verifier_commande() // Termine le programme si la variable globale saisie = T, renvoie false si saisie = Q, affiche les instructions si saisie = I, et renvoie true si saisie != Q et != T
{
    if (strcmp(saisie, "Q") == 0)
    {
        return false; // Revient en arrière, quitte la partie ou termine l'exécution du programme selon le contexte
    }
    else if (strcmp(saisie, "T") == 0)
    {
        printf("Fin du programme.\n");
        exit(0); // Termine l'éxecution du programme
    }
    else if (strcmp(saisie, "I") == 0)
    {
        afficher_instructions(); // Affiche les instructions
        return true;
    }
    return true; // Si aucune commande spéciale n'est saisie, le jeu continue
}

bool verifier_format_coordonnees() // Vérifie que les coordonnées saisies (stockées dans la variable globale saisie) sont bien de la forme "y-x"
{
    return strlen(saisie) == 3 && isdigit(saisie[0]) && saisie[1] == '-' && isdigit(saisie[2]);
}

bool verifier_position_valide(int pos_y, int pos_x, char orientation, int longueur, char grille[10][10]) // Vérifie que la position y-x et l'orientation passée en paramètre sont valides lors du placement d'un navire
{
    int dy = 0, dx = 0;
    switch (orientation) // Permet de déterminer le déplacement en y et en x selon l'orientation
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
        if (ny < 0 || ny > 9 || nx < 0 || nx > 9 || grille[ny][nx] == 'N') // Vérifie que le navire ne sort pas de la grille et ne chevauche pas un autre navire
        {
            return false;
        }
    }
    return true;
}

bool verifier_tir_utile(int x, int y, char grille_tirs[10][10]) // Vérifie si le tir y-x est utile (case non encore touchée) lors des tirs de l'ordinateur
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

/* Fonctions d'affichage */

bool afficher_instructions() // Affiche les instructions du jeu
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
        printf("Entrez Q pour fermer les instructions : ");
        scanf("%s", saisie);
        if (!verifier_commande())
            return false;
    } while (strcmp(saisie, "Q") != 0);
    return true; // Retourne au menu principal ou à la partie en cours
}

void afficher_ligne_grille(char grille[10][10], int i) // Affiche la ligne d'indice i de la grille 10*10 passée en paramètre
{
    for (int j = 0; j < 10; j++)
    {
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

void afficher_grille(char grille[10][10]) // Affiche la grille passée en paramètre
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

void affichage_placements(int niveau, int indice, char type[15], int longueur, char nom[30], bool aleatoire) // Affiche les textes introduisant le placement d'un navire selon le mode de jeu (mode IA ou non)
{
    if (niveau != 0 && indice == 1 && aleatoire) // Niveau != 0 -> Mode IA
    {
        if (longueur == 5)
            printf("\nPlacement du %s", type);
        else
            printf(", du %s", type);
        if (longueur == 2)
            printf(".\n");
    }
    else if (niveau != 0 && indice == 1)
    {
        printf("\nPlacement du %s (%d cases).\n", type, longueur);
    }
    else if (niveau == 0) // Niveau = 0 -> Mode Joueur contre Joueur
    {
        printf("\nPlacement du %s (%d cases) pour %s.\n", type, longueur, nom);
    }
}

void afficher_noms_joueurs(int att_indice, char att_nom[30], char def_nom[30]) // Affiche les noms des joueurs : cette fonction est utilisée lors de l'affichage des grilles
{
    char nomA[50], nomB[50];   // Augmente la taille de 20 caractères pour tenir compte de la concaténation
    char tempA[50], tempB[50]; // Chaînes temporaires pour la concaténation

    strcpy(nomA, (att_indice == 1) ? att_nom : def_nom); // nomA = attaquant si att_indice = 1, sinon nomA = defenseur
    strcpy(nomB, (att_indice == 1) ? def_nom : att_nom);

    /* Concatène "Tirs de " avec les noms */
    strcpy(tempA, "Tirs de ");
    strcat(tempA, nomA); // "Tirs de attaquant"
    strcpy(tempB, "Tirs de ");
    strcat(tempB, nomB); // "Tirs de defenseur"

    /* Copie le résultat final dans nomA et nomB */
    strcpy(nomA, tempA);
    strcpy(nomB, tempB);

    int largeur = 35; // Largeur d'une grille avec espacements compris
    int longueur_nomA = strlen(nomA);
    int longueur_nomB = strlen(nomB);

    int espacesAvantNomA = (largeur - longueur_nomA) / 2;
    int espacesAvantNomB = (largeur - longueur_nomB) / 2;

    printf("\n%*s%s%*s", espacesAvantNomA, "", nomA, espacesAvantNomA, ""); // Centrer le nomA
    printf("%*s%s\n", espacesAvantNomB, "", nomB);                          // Centrer le nomB
}

void afficher_type_partie(int niveau) // Affiche le type de partie sélectionné (classique ou contre l'ordinateur)
{
    if (niveau != 0)
        printf("\nPartie contre l'ordinateur : niveau %d\n", niveau);
}

/* Fonctions de saisie utilisateur */

void demander_coordonnees(int *pos_y, int *pos_x) // Demande des coordonnées au joueur et les stocke dans pos_x et pos_y si elles sont valides
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

void demander_orientation(char *orientation) // Demande une orientation au joueur et la stocke dans orientation si elle est valide (N, S, O ou E)
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

void initialiser_nom(char nom[30], int niveau, int indice) // Demande le nom du joueur ou initialise le nom du joueur 2 en "L'ordinateur" pour le mode IA
{
    if (niveau == 0 || indice == 1)
    {
        printf("Entrez votre nom : ");
        scanf("%s", nom);
        // La saisie du nom ne se fait pas directement dans la variable saisie car il a une taille potentiellement supérieure à 3 caractères

        saisie[0] = nom[0];
        saisie[1] = nom[1];
        // Néanmoins, on utilise la variable saisie pour vérifier qu'une commande globale n'est pas rentrée à la place du nom

        if (!verifier_commande())
        {
            return;
        }
    }
    else
    {
        strcpy(nom, "L'ordinateur");
    }
}

/* Fonctions opérationnelles */

void ajoute_delai() // Ajoute un délai de 500ms
{
    struct timespec ts;
    ts.tv_sec = 0;
    ts.tv_nsec = 500000000L; // 500ms
    nanosleep(&ts, NULL);
}

void remplir_grille(char grille[10][10]) // Remplit la grille passée en paramètre de points (utilisée lors de l'initialisation des grilles)
{
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            grille[i][j] = '.';
        }
    }
}

void melanger_liste(int ordre_tir[4]) // Mélange les directions possibles pour les tirs pour mettre en place un ordre aléatoire
{
    for (int i = 0; i < 4; i++)
    {
        int j = rand() % 4;
        int temp = ordre_tir[i];
        ordre_tir[i] = ordre_tir[j];
        ordre_tir[j] = temp;
    }
}

bool trouver_tir_IA2(int *x, int *y, int *last_x, int *last_y, char grille_tirs[10][10]) // Trouve tir le plus au Sud-Est non encore complètement entouré de tirs passés et met ses coordonnées dans x et y
{
    bool trouver = false;
    for (int ligne_y = 0; ligne_y <= *last_y; ligne_y++)
    {
        for (int colonne_x = 0; colonne_x < 10; colonne_x++)
        {
            if ((grille_tirs[ligne_y][colonne_x] == 'X') && !((ligne_y >= *last_y) && (colonne_x >= *last_x)))
            {
                *x = colonne_x;
                *y = ligne_y;
                trouver = true;
            }
        }
    }
    return trouver;
}

void trouver_coordonnee_tir_IA(int ordre_tir[4], int numero_tir, int direction, int *x_tir, int *y_tir, int x, int y) // Détermine les coordonnées d'un tir à proximité en fonction des paramètres passés
{
    if (ordre_tir[numero_tir] == 0 && direction != 1) // N
    {
        *x_tir = x;
        *y_tir = y - 1;
    }
    else if (ordre_tir[numero_tir] == 1 && direction != 2) // E
    {
        *x_tir = x + 1;
        *y_tir = y;
    }
    else if (ordre_tir[numero_tir] == 2 && direction != 1) // S
    {
        *x_tir = x;
        *y_tir = y + 1;
    }
    else if (ordre_tir[numero_tir] == 3 && direction != 2) // O
    {
        *x_tir = x - 1;
        *y_tir = y;
    }
    else
    {
        *x_tir = 10;
        *y_tir = 10;
    }
}

void detecte_direction_navire(char grille_tirs[10][10], int x_tir, int y_tir, int *direction) // Détermine la direction du navire en fonction des tirs déjà effectués
{
    int yplus = y_tir + 1, ymoins = y_tir - 1, xplus = x_tir + 1, xmoins = x_tir - 1;
    if ((grille_tirs[y_tir][xmoins] == 'X' || grille_tirs[y_tir][xplus] == 'X') && xmoins >= 0 && xplus < 10)
    {
        *direction = 1; // E ou W
    }
    else if ((grille_tirs[ymoins][x_tir] == 'X' || grille_tirs[yplus][x_tir] == 'X') && ymoins >= 0 && yplus < 10)
    {
        *direction = 2; // N ou S
    }
    else
    {
        *direction = 0; // toute direction
    }
}

bool navire_imcomplet_sur_direction(char grille_tirs[10][10], int x, int y, int direction) // Vérifie si un navire est incomplet sur une direction donnée
{
    int x1 = x, y1 = y;
    if (direction == 1)
    {
        for (; (grille_tirs[y][x1] == 'X') && (x1 < 10); x1++)
        {
        }
        if (grille_tirs[y][x1] == '.')
        {
            return false;
        }
        x1 = x;
        for (; (grille_tirs[y][x1] == 'X') && (x1 > 0); x1--)
        {
        }
        if (grille_tirs[y][x1] == '.')
        {
            return false;
        }
    }
    else if (direction == 2)
    {
        for (; (grille_tirs[y1][x] == 'X') && (y1 < 10); y1++)
        {
        }
        if (grille_tirs[y][x] == '.')
        {
            return false;
        }
        y1 = y;
        for (; (grille_tirs[y1][x] == 'X') && (y1 > 0); y1--)
        {
        }
        if (grille_tirs[y1][x] == '.')
        {
            return false;
        }
    }
    return true;
}

bool selectionner_tir(int *x, int *y, char grille_tirs[10][10], int niveau) // À partir d'une position, teste les positions adjacentes de manière aléatoire jusqu'à trouver un tir
{
    int y_tir, x_tir, ordre_tir[4] = {0, 1, 2, 3};
    melanger_liste(ordre_tir);

    if (niveau == 2)
    {
        for (int numero_tir = 0; numero_tir < 4; numero_tir++)
        {
            trouver_coordonnee_tir_IA(ordre_tir, numero_tir, 0, &x_tir, &y_tir, *x, *y);
            if (verifier_tir_utile(x_tir, y_tir, grille_tirs))
            {
                *x = x_tir;
                *y = y_tir;
                return true;
            }
        }
        return false;
    }
    else
    {
        int direction_tir;
        detecte_direction_navire(grille_tirs, *x, *y, &direction_tir);
        for (int numero_tir = 0; numero_tir < 4; numero_tir++)
        {
            trouver_coordonnee_tir_IA(ordre_tir, numero_tir, direction_tir, &x_tir, &y_tir, *x, *y);
            if (verifier_tir_utile(x_tir, y_tir, grille_tirs) && x_tir >= 0 && x_tir < 10 && y_tir >= 0 && y_tir < 10)
            {
                *x = x_tir;
                *y = y_tir;
                return true;
            }
        }
        if (direction_tir != 0)
        {
            if (navire_imcomplet_sur_direction(grille_tirs, *x, *y, direction_tir))
            {
                for (int numero_tir = 0; numero_tir < 4; numero_tir++)
                {
                    trouver_coordonnee_tir_IA(ordre_tir, numero_tir, 0, &x_tir, &y_tir, *x, *y);
                    if (verifier_tir_utile(x_tir, y_tir, grille_tirs) && x_tir >= 0 && x_tir < 10 && y_tir >= 0 && y_tir < 10)
                    {
                        *x = x_tir;
                        *y = y_tir;
                        return true;
                    }
                }
            }
        }
        return false;
    }
}