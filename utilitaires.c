#include "utilitaires.h"

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