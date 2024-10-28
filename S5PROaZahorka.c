/*Projet FIPMIK3 - Augustin ZAHORKA - Bataille Navale*/

// Gestion des bibiliothèques
#include <stdio.h>
#include <math.h>
#include <stdbool.h> // Manipuler des booléens
#include <string.h>  // Manipuler les chaînes de caractères
#include <stdlib.h>  // Manipuler les fonctions propres à l'aléatoire
#include <unistd.h>  // Fonctions sleep() qui me permet de fluidifier les interractions avec l'ordinateur
#include <time.h>    // Pour intégrer les fonctions aléatoires essentiellement
#include <ctype.h>   // Pour isdigit()

// Gestion des couleurs
#define RESET "\x1b[0m"
#define BLEU "\x1b[34m"
#define ROUGE "\x1b[31m"
#define VERT "\x1b[32m"

typedef struct
{
    int longueur, pos_y, pos_x;
    ;
    char type[15];    // Torpilleur, croiseur, etc.
    bool etat;        // Me sert pour optimiser quelques fonctions, mais ne joue pas un rôle clé dans mon programme
    char orientation; // N pour Nord, E pour Est, S pour Sud, O pour Ouest
} Navire;             // Structure qui me permet de simplifier l'accès aux données des différents navires

typedef struct
{
    int indice;               // 1 ou 2, pour simplifier l'affichage des grilles
    char grille[10][10];      // Grille qui contient les vraies données sur les navires du joueur
    char grille_tirs[10][10]; // Grille qui contient les données sur les précédentes tentatives de tirs du joueur
    Navire navires[5];        // Liste des 5 navires du joueur
    char nom[30];             // Nom du joueur, demandé en début de partie
} Joueur;

bool instructions(char select[3]);

int est_coordonnees(char select[3])
{
    return strlen(select) == 3 && isdigit(select[0]) && select[1] == '-' && isdigit(select[2]);
}

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

Navire creer_navire(int indice_navire, Joueur *joueur, char select[3], bool aleatoire)
{
    char types_navires[5][100] = {"Porte-avions", "Croiseur", "Destroyer", "Sous-marin", "Torpilleur"};
    int longueurs_navires[5] = {5, 4, 3, 3, 2};
    char directions[4] = {'N', 'S', 'O', 'E'};

    Navire navire;
    strcpy(navire.type, types_navires[indice_navire]);
    navire.longueur = longueurs_navires[indice_navire];
    navire.etat = true;                                                                           // Le navire est en bon état à sa création (n'est pas coulé)
    printf("\nPlacement du %s (%d cases) pour %s.\n", navire.type, navire.longueur, joueur->nom); // Affichage

    do
    {
        if (aleatoire)
        {
            navire.pos_y = rand() % 10;
            navire.pos_x = rand() % 10;
            navire.orientation = directions[indice_navire % 4];
        }
        else
        {
            do
            {
                printf("\nEntrez la position Y-X (Ligne-Colonne) : ");
                scanf("%s", select); // On utilise select pour accueillir toutes les saisies de l'utilisateur, afin de mettre en place des commandes universelles
                if (!verifier_commande(select))
                    return navire;
            } while (!est_coordonnees(select));
            sscanf(select, "%d-%d", &navire.pos_y, &navire.pos_x);

            do
            {
                printf("Entrez l'orientation (N, S, O, E) : ");
                scanf("%s", select);
                if (!verifier_commande(select))
                    return navire; // Vérifie commande après saisie
            } while (strcmp(select, "N") != 0 && strcmp(select, "S") != 0 && strcmp(select, "O") != 0 && strcmp(select, "E") != 0);
            sscanf(select, "%c", &navire.orientation);

            if (!position_valide(navire.pos_y, navire.pos_x, navire.orientation, navire.longueur, joueur->grille))
                printf("\nPosition et orientation invalides.\n");
        }
    } while (!position_valide(navire.pos_y, navire.pos_x, navire.orientation, navire.longueur, joueur->grille));

    if (aleatoire)
        sleep(1);

    return navire;
}

Navire creer_navire_IA(int indice_navire, Joueur *joueur, char select[3], bool aleatoire)
{
    char types_navires[5][100] = {"Porte-avions", "Croiseur", "Destroyer", "Sous-marin", "Torpilleur"};
    int longueurs_navires[5] = {5, 4, 3, 3, 2};
    char directions[4] = {'N', 'S', 'O', 'E'};

    Navire navire;
    strcpy(navire.type, types_navires[indice_navire]);
    navire.longueur = longueurs_navires[indice_navire];
    navire.etat = true;
    if (joueur->indice==1)
    {
        printf("\nPlacement du %s (%d cases).\n", navire.type, navire.longueur); // Affichage
    }

    do
    {
        if (aleatoire)
        {
            navire.pos_y = rand() % 10;
            navire.pos_x = rand() % 10;
            navire.orientation = directions[indice_navire % 4];
        }
        else
        {
            do
            {
                printf("\nEntrez la position Y-X : ");
                scanf("%s", select); // On utilise select pour accueillir toutes les saisies de l'utilisateur, afin de mettre en place des commandes universelles
                if (!verifier_commande(select))
                    return navire;
            } while (!est_coordonnees(select));
            sscanf(select, "%d-%d", &navire.pos_y, &navire.pos_x);

            do
            {
                printf("Entrez l'orientation (N, S, O, E) : ");
                scanf("%s", select);
                if (!verifier_commande(select))
                    return navire; // Vérifie commande après saisie
            } while (strcmp(select, "N") != 0 && strcmp(select, "S") != 0 && strcmp(select, "O") != 0 && strcmp(select, "E") != 0);
            sscanf(select, "%c", &navire.orientation);

            if (!position_valide(navire.pos_y, navire.pos_x, navire.orientation, navire.longueur, joueur->grille))
                printf("\nPosition et orientation invalides.\n");
        }
    } while (!position_valide(navire.pos_y, navire.pos_x, navire.orientation, navire.longueur, joueur->grille));

    if (aleatoire && joueur->indice==1)
        sleep(1);

    return navire;
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

void placer_navires(Joueur *joueur, char select[3])
{
    bool aleatoire = false;
    do
    {
        printf("\nVoulez-vous que l'ordinateur place vos navires ?\n");
        printf("O (Oui) ou N (Non) : ");
        scanf("%s", select);
        if (!verifier_commande(select))
        {
            return;
        }
    } while (strcmp(select, "Q") == 0);

    if (strcmp(select, "O") == 0)
    {
        aleatoire = true;
    }
    for (int i = 0; i < 5; i++)
    {
        joueur->navires[i] = creer_navire(i, joueur, select, aleatoire);
        if (strcmp(select, "Q") == 0)
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
        if (!aleatoire || i == 4)
            afficher_grille(joueur->grille);
    }
}

Joueur initialiser_joueur(char select[3], int indice)
{
    Joueur joueur;
    remplir_grille(joueur.grille); // Pas besoin de pointeur car la grille est un tableau
    remplir_grille(joueur.grille_tirs);
    joueur.indice=indice;
    printf("\n--- Initialisation du Joueur %d ---\n", indice);
    do
    {
        printf("Entrez votre nom : ");
        scanf("%s", select);
        if (!verifier_commande(select))
        {
            return joueur;
        }
    } while (strcmp(select, "Q") == 0);

    strcpy(joueur.nom, select);      // Utilise strcpy pour copier le nom
    placer_navires(&joueur, select); // Pointeur pour que les navires soient référencés dans les attributs du joueur
    return joueur;
    
}

void placer_navires_IA(Joueur *joueur, char select[3])
{
    bool aleatoire = false;
    if (joueur->indice==1){
        do
        {
            printf("\nVoulez-vous que l'ordinateur place vos navires ?\n");
            printf("O (Oui) ou N (Non) : ");
            scanf("%s", select);
            if (!verifier_commande(select))
            {
                return;
            }
        } while (strcmp(select, "Q") == 0);

        if (strcmp(select, "O") == 0)
        {
            aleatoire = true;
        }
    }
    if (joueur->indice==2) aleatoire=true;

    for (int i = 0; i < 5; i++)
    {
        joueur->navires[i] = creer_navire_IA(i, joueur, select, aleatoire);
        if (strcmp(select, "Q") == 0)
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
        if (!aleatoire || (i == 4 && joueur->indice==1))
            afficher_grille(joueur->grille);
    }
}

Joueur initialiser_joueur_IA(char select[3], int indice)
{
    Joueur joueur;
    joueur.indice=indice;
    remplir_grille(joueur.grille); // Pas besoin de pointeur car la grille est un tableau
    remplir_grille(joueur.grille_tirs);
    if (indice==1) {
        printf("\n--- Initialisation du Joueur ---\n");
        strcpy(joueur.nom,"Joueur");
    }
    else{
        strcpy(joueur.nom,"l'ordinateur");
    }
    placer_navires_IA(&joueur, select); // Pointeur pour que les navires soient référencés dans les attributs du joueur
    return joueur;
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



void afficher_les_noms(Joueur *attaquant, Joueur *defenseur)
{
    char nomA[50], nomB[50];   // Augmenter la taille pour tenir compte de la concaténation
    char tempA[50], tempB[50]; // Chaînes temporaires pour la concaténation
    if (attaquant->indice == 1)
    {
        strcpy(nomA, attaquant->nom);
        strcpy(nomB, defenseur->nom);
    }
    else
    {
        strcpy(nomA, defenseur->nom);
        strcpy(nomB, attaquant->nom);
    }
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

void afficher_grilles(Joueur *attaquant, Joueur *defenseur) // Affichage des grilles de tirs des deux joueurs côte-côte
{
    char(*grilleA)[10], (*grilleB)[10]; // Pour considérer Grilles A et B comme des pointeurs.

    if (attaquant->indice == 1)
    {
        grilleA = attaquant->grille_tirs;
        grilleB = defenseur->grille_tirs;
    }
    else
    {
        grilleA = defenseur->grille_tirs;
        grilleB = attaquant->grille_tirs;
    }
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
    afficher_les_noms(attaquant, defenseur);
}

void maj_grille_tirs(Joueur *defenseur, Joueur *attaquant, int indice)
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

bool etat_navire(Joueur *defenseur, int indice_navire, int indiceY, int indiceX)
{
    for (int j = 0; j < defenseur->navires[indice_navire].longueur; j++)
    {
        if (defenseur->grille[defenseur->navires[indice_navire].pos_y + j * indiceY][defenseur->navires[indice_navire].pos_x + j * indiceX] == 'N')
        {
            return true;
        }
    }
}

void update_navires(Joueur *attaquant, Joueur *defenseur)
{
    for (int i = 0; i < 5; i++)
    {
        if (defenseur->navires[i].etat)
        {
            bool navire_ok = false;
            if (defenseur->navires[i].orientation == 'N')
            {
                navire_ok = etat_navire(defenseur, i, -1, 0);
            }
            else if (defenseur->navires[i].orientation == 'S')
            {
                navire_ok = etat_navire(defenseur, i, 1, 0);
            }
            else if (defenseur->navires[i].orientation == 'O')
            {
                navire_ok = etat_navire(defenseur, i, 1, -1);
            }
            else if (defenseur->navires[i].orientation == 'E')
            {
                navire_ok = etat_navire(defenseur, i, 0, 1);
            }
            if (!navire_ok)
            {
                defenseur->navires[i].etat = false;
                maj_grille_tirs(defenseur, attaquant, i);
                afficher_grilles(attaquant, defenseur);
                printf("\nLe %s de %s a atteint le fond.\n", defenseur->navires[i].type, defenseur->nom);
                return;
            }
            else if (i==4) afficher_grilles(attaquant, defenseur);
        }
    }
}

bool tirer(Joueur *attaquant, Joueur *defenseur, char select[3])
{
    int y, x;
    do
    {
        printf("\n%s, entrez la localisation de tir (Y-X) : ", attaquant->nom);
        scanf("%s", select);
        if (!verifier_commande(select))
            break;
        sscanf(select, "%d-%d", &y, &x);
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
        printf("\nDans l'eau... !\n");
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

bool tir_utile(int x, int y, Joueur *attaquant){
    if (attaquant->grille_tirs[y][x]=='.'){
        return true;
    }
}

bool tirer_IA_1(Joueur *attaquant, Joueur *defenseur, char select[3])
{
    sleep(2);
    int y, x;
    do
    {
        y = rand() % 10;
        x = rand() % 10;
    } while (!tir_utile(x,y,attaquant)); // Vérifie que le tir n'a pas déjà été tenté

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
        printf("\nDans l'eau... !\n");
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

bool joueur_a_perdu(Joueur *joueur)
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

void lancer_tours(Joueur *joueur1, Joueur *joueur2, char select[3], bool IA)
{
    int tours = 0;
    bool touche;
    bool rebelote = false;
    while (true)
    {
        if (tours % 2 == 0)
        {
            if (!rebelote)
                if (IA) printf("\n--- Votre tour ---\n", joueur1->nom);
                else printf("\n--- Tour de %s ---\n", joueur1->nom);
            touche = tirer(joueur1, joueur2, select); // Joueur 1 tire sur Joueur 2
            if (strcmp(select, "Q") == 0)
                return;
        }
        else
        {
            if (!rebelote)
                printf("\n--- Tour de %s ---\n", joueur2->nom);
            if (IA) touche = tirer_IA_1(joueur2, joueur1, select); // Joueur 2 tire sur Joueur 1
            else {
                touche = tirer(joueur2, joueur1, select); // Joueur 2 tire sur Joueur 1
                if (strcmp(select, "Q") == 0)
                    return;
            }
        }

        // Ajoute une fonction pour vérifier si un joueur a perdu
        if (joueur_a_perdu(joueur2))
        {
            printf("\n%s est vainqueur de cette partie !\n", joueur1->nom);
            break;
        }
        else if (joueur_a_perdu(joueur1))
        {
            printf("\n%s est vainqueur de cette partie !\n", joueur2->nom);
            break;
        }

        if (!touche)
        {
            tours++;
            rebelote = false;
        }
        else
        {
            rebelote = true;
        }
    }
}

void lancer_partie(char select[3])
{
    Joueur joueur1 = initialiser_joueur(select, 1);
    if (strcmp(select, "Q") == 0)
        return; // Si joueur 1 quitte

    Joueur joueur2 = initialiser_joueur(select, 2);
    if (strcmp(select, "Q") == 0)
        return; // Si joueur 2 quitte

    lancer_tours(&joueur1, &joueur2, select, false);
}

void partie_IA_1(char select[3])
{
    Joueur joueur1 = initialiser_joueur_IA(select, 1);
    if (strcmp(select, "Q") == 0)
        return; // Si joueur 1 quitte

    Joueur joueur2 = initialiser_joueur_IA(select, 2);
    if (strcmp(select, "Q") == 0)
        return; // Si joueur 2 quitte

    printf("Joueur 1 et 2 : %s %s\n", joueur1.nom, joueur2.nom);

    lancer_tours(&joueur1, &joueur2, select, true);
}

void partie_IA_2(char select[3]) // Pas encore opérationnelle
{
    Joueur joueur1 = initialiser_joueur(select, 1);
    if (strcmp(select, "Q") == 0)
        return; // Si joueur 1 quitte

    Joueur joueur2 = initialiser_joueur(select, 2);
    if (strcmp(select, "Q") == 0)
        return; // Si joueur 2 quitte

    lancer_tours(&joueur1, &joueur2, select, true);
}

void partie_IA_3(char select[3]) // Pas encore opérationnelle
{
    Joueur joueur1 = initialiser_joueur(select, 0);
    if (strcmp(select, "Q") == 0)
        return; // Si joueur 1 quitte

    Joueur joueur2 = initialiser_joueur(select, 1);
    if (strcmp(select, "Q") == 0)
        return; // Si joueur 2 quitte

    lancer_tours(&joueur1, &joueur2, select, true);
}

void choix_niveau(char select[3]){
    printf("\n  1. Niveau 1 : l'ordinateur joue en mode random");
    printf("\n  2. Niveau 2 : l'ordinateur utilise quelques techniques");
    printf("\n  3. Niveau 3 : l'ordinateur utilise des algorithmes plus intelligents");
    printf("\n  Q. Revenir au menu principal");
    printf("\n\nVotre choix : ");
    scanf("%s", select);

    if (!verifier_commande(select))
        return; // Quitte le programme si "T" est entré

    if (strcmp(select, "1") == 0)partie_IA_1(select);
    else if (strcmp(select, "2") == 0)partie_IA_2(select);
    else if (strcmp(select, "3") == 0)partie_IA_3(select);

}

void menu_principal(char select[3])
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
        scanf("%s", select);

        // Vérifie si l'utilisateur entre une commande spéciale
        if (!verifier_commande(select))
            return; // Quitte le programme si "T" est entré
        if (strcmp(select, "P") == 0)
            lancer_partie(select);
        
        if (strcmp(select, "O") == 0){
            choix_niveau(select);
        }
    } while (strcmp(select, "T") != 0);
}

// Fonction main()

int main()
{
    srand(time(NULL));
    char select[3];
    menu_principal(select);
    return 0; // Terminer après la partie
}
