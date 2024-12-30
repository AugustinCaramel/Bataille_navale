#include "game.h"

zone initialiser_zone(char nom[30],int x,int y,int x1,int y1)
{
    int compteur = 0;
    zone zone;
    strcpy(zone.nom, nom);
    zone.x = x;
    zone.y = y;
    zone.x1 = x1;
    zone.y1 = y1;
    zone.taille_cible[0] = 0;
    zone.taille_cible[1] = 1;
    zone.tir_libre = true;

    for (int x_ = x; x_ <= x1; x_++)
    {
        for (int y_ = y; y_ <= y1; y_++)
        {
            if((x_ % 2 == 0 && y_ % 2 != 0) || (x_ % 2 != 0 && y_ % 2 == 0))
            {
                zone.case_libre[compteur][0] = x_;
                zone.case_libre[compteur][1] = y_;
                compteur = compteur +1;
            }
        }
        
    }
    for (int i = compteur; i < 8; i++)
    {                
        zone.case_libre[compteur][0] = 20;
        zone.case_libre[compteur][1] = 20;
    }
    
    

    return zone;
}


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
    int zone_choisi,ecart_x,ecart_y,x1,y1;

    zone zone[8];
    zone[0]=initialiser_zone("haut_gauche",1,1,4,4);
    zone[1]=initialiser_zone("haut_droit",5,1,8,4);
    zone[2]=initialiser_zone("bas_gauche",1,5,4,8);
    zone[3]=initialiser_zone("bas_droit",5,5,8,8);
    zone[4]=initialiser_zone("bord_gauche",0,1,0,8);
    zone[5]=initialiser_zone("bord_droit",9,1,9,8);
    zone[6]=initialiser_zone("bord_bas",1,9,8,9);
    zone[7]=initialiser_zone("bord_haut",1,0,8,0);


    for (int indice = 0; indice < 5; indice++)
    {
        if (defenseur->navires[indice].etat == false)
        {
            //flotte_intacte = false;
            break;
        } 
    }

    zone_choisi = choix_zone_et_navire(defenseur->navires,zone,attaquant->grille_tirs);

    while (choix_tire_reperage_IA3(zone[zone_choisi],attaquant->grille_tirs,&*x,&*y) == false)
    {
        zone[zone_choisi].tir_libre = false;
        printf("%d eclu_reperage/ ",zone_choisi);
        zone_choisi = choix_zone_et_navire(defenseur->navires,zone,attaquant->grille_tirs);
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


bool choix_tire_reperage_IA3(zone zone,char grille_tirs[10][10],int *x,int *y)
{
    int taille,ecart_x,ecart_y,liste_tir[8][2],num_tir,nbr_tir = 0;
    char direction;
    bool tir_rate = false;

    if (zone.taille_cible[0] >= zone.taille_cible[1])
    {
        taille = zone.taille_cible[0];
        direction = 'X';
    }
    else if (zone.taille_cible[0] < zone.taille_cible[1])
    {
        taille = zone.taille_cible[1];
        direction = 'Y';
    }
    printf("adopte %c %d %d/ ",direction,zone.taille_cible[0],zone.taille_cible[1]);

    updat_case_libre(zone.case_libre,grille_tirs);
    generer_liste_tir(zone,liste_tir,&nbr_tir);    

    do
    {
        if (nbr_tir == 0)
        {
            printf("nbr tir 0/ ");
            return false;
        }
        num_tir = rand() % nbr_tir;
        *x = liste_tir[num_tir][0];
        *y = liste_tir[num_tir][1];
        printf("compatible:%c %d %d/ ",direction,*x,*y);
        if (verifier_taille_compatible(grille_tirs,direction,taille,*x,*y) == false)
        {
            supprimer_tir(liste_tir,num_tir,&nbr_tir);
            tir_rate = true;
        }
        else
        {
            tir_rate = false;
        }
    } while (tir_rate == true);    

    printf("xy:%d %d/ ",*x,*y);
    return true;
}


void updat_case_libre(int case_libre[8][2],char grille_tirs[10][10])
{
    for (int i = 0; i < 8 && case_libre[i][0] != 20 && case_libre[i][1] != 20; i++)
    {
        if (grille_tirs[(case_libre[i][1])][(case_libre[i][0])] != '.')
        {
            case_libre[i][0] = 10;
            case_libre[i][1] = 10;
        }
    }
}


void generer_liste_tir(zone zone,int liste_tir[8][2],int *nbr_tir)
{
    int num_tir; 
    for (int i = 0; i < 8 && zone.case_libre[i][0] != 20 && zone.case_libre[i][1] != 20; i++)
    {
        if (zone.case_libre[i][0] != 10 && zone.case_libre[i][1] != 10)
        {
            liste_tir[*nbr_tir][0] = zone.case_libre[i][0];
            liste_tir[*nbr_tir][1] = zone.case_libre[i][1];
            *nbr_tir = *nbr_tir + 1;
        }
    }
}


void supprimer_tir(int liste_tir[8][2],int num_tir,int *nbr_tir)
{
    for (int i = num_tir; i < *nbr_tir; i++)
    {
        liste_tir[i][0] = liste_tir[i+1][0];
        liste_tir[i][1] = liste_tir[i+1][1];
    }
    liste_tir[*nbr_tir][0] = 10;
    liste_tir[*nbr_tir][1] = 10;
    *nbr_tir = *nbr_tir - 1;
    
}


bool verifier_taille_compatible(char grille_tirs[10][10],char direction,int taille,int x,int y)
{
    int comptage = 0;

    if ((x % 2 == 0 && y % 2 == 0) || (x % 2 != 0 && y % 2 != 0) || grille_tirs[y][x] != '.')
    {
        return false;
    }
    printf("\netape1 taille:%d/",taille);

    if (direction == 'X')
    {
        for (int i = x;grille_tirs[y][i] == '.' && i >= 0 && i <= 9 && comptage < taille; i++)
        {
            comptage = comptage + 1;
            printf("comptage:%d %d %d/ ",comptage,i,y);
        }
        for (int i = x-1;grille_tirs[y][i] == '.' && i >= 0 && i <= 9 && comptage < taille; i--)
        {
            comptage = comptage + 1;
            printf("comptage:%d %d %d/ ",comptage,i,y);
        } 
    }
    else if (direction == 'Y')
    {
        for (int i = y;grille_tirs[i][x] == '.' && i >= 0 && i <= 9 && comptage < taille; i++)
        {
            comptage = comptage + 1;
            printf("comptage:%d %d %d/ ",comptage,x,i);
        }
        for (int i = y-1;grille_tirs[i][x] == '.' && i >= 0 && i <= 9 && comptage < taille; i--)
        {
            comptage = comptage + 1;
            printf("comptage:%d %d %d/ ",comptage,x,i);
        } 
    }
    printf("\n");

    if (comptage >= taille)
    {
        return true;
    }
    else
    {
        return false;
    }
}


int choix_zone_et_navire(Navire navires[5],zone zone[8],char grille_tirs[10][10])
{
    int meilleur_zone,flotte_estime_x[8][4],flotte_estime_y[8][4];
    float stat,stat_meilleur_zone = 0,nbr_ratee,nbr_cases;
    for (int num_zone = 0; num_zone < 8; num_zone++)
    {
        estime_flotte_par_zone(navires,zone[num_zone],grille_tirs,flotte_estime_x[num_zone],flotte_estime_y[num_zone]);
        zone[num_zone].taille_cible[0] = estime_taile_navire_cible(flotte_estime_x[num_zone]);
        zone[num_zone].taille_cible[1] = estime_taile_navire_cible(flotte_estime_y[num_zone]);

        printf("zone %d/ ",num_zone);
        printf("estimex:%d %d %d %d /",flotte_estime_x[num_zone][0],flotte_estime_x[num_zone][1],flotte_estime_x[num_zone][2],flotte_estime_x[num_zone][3]);
        printf("estimey:%d %d %d %d /",flotte_estime_y[num_zone][0],flotte_estime_y[num_zone][1],flotte_estime_y[num_zone][2],flotte_estime_y[num_zone][3]);
        printf("taille cible:%d %d/ ",zone->taille_cible[0],zone->taille_cible[1]);
        printf("\n");
    }
    meilleur_zone=choix_zone_favorable(flotte_estime_x,flotte_estime_y,zone,grille_tirs);
    estime_direction_cible(&zone[meilleur_zone],flotte_estime_x[meilleur_zone],flotte_estime_y[meilleur_zone]);
    printf("\nzone choisis:%d/ ",meilleur_zone);
    printf("taille cible:%d %d/ ",zone[meilleur_zone].taille_cible[0],zone[meilleur_zone].taille_cible[1]);
    return meilleur_zone;
}
    

int choix_zone_favorable(int flotte_estime_x[8][4],int flotte_estime_y[8][4],zone zone[8],char grille_tirs[10][10])
{
    bool restrain_x[8] = {true,true,true,true,true,true,true,true},restrain_y[8] = {true,true,true,true,true,true,true,true};
    int nbr_restrain_x = 0,nbr_restrain_y = 0,zone_choisi_x = 0,zone_choisi_y = 0,nbr_navire_x,nbr_navire_y;

    controle_zone_disponible(zone,grille_tirs);
    for (int num_zone = 0;num_zone < 8;num_zone++)
    {
        if (zone[num_zone].tir_libre == false)
        {
            restrain_x[num_zone] = false;
            nbr_restrain_x = nbr_restrain_x + 1;
            restrain_y[num_zone] = false;
            nbr_restrain_y = nbr_restrain_y + 1;
            printf("exclu %d/ ",num_zone);
        }
    }

    for (int navire = 0;navire < 4;navire++)
    {
        printf("X ");
        nbr_navire_x = 0;
        nbr_navire_y = 0;
        for (int num_zone = 0;num_zone < 8;num_zone++)
        {
            printf("X ");
            restrain_choix(flotte_estime_x,restrain_x,num_zone,navire,&nbr_navire_x,&nbr_restrain_x);
            printf("y ");
            restrain_choix(flotte_estime_y,restrain_y,num_zone,navire,&nbr_navire_y,&nbr_restrain_y);
        }
    }
    choisi_zone(restrain_x,restrain_y,&zone_choisi_x,&zone_choisi_y);
    for (int num_navire = 0; num_navire < 4; num_navire++)
    {
        if (flotte_estime_x[zone_choisi_x][num_navire] > flotte_estime_y[zone_choisi_y][num_navire])
        {
            return zone_choisi_x;
        }
        else if(flotte_estime_x[zone_choisi_x][num_navire] < flotte_estime_y[zone_choisi_y][num_navire])
        {
            return zone_choisi_y;
        }
        else if(num_navire == 3)
        {
            return zone_choisi_x;
        }
    }
}


void controle_zone_disponible(zone zone[8],char grille_tirs[10][10])
{
    bool trouve;
    for (int num_zone = 0; num_zone < 8; num_zone++)
    {
        trouve = false;
        if(zone[num_zone].tir_libre == true)
        {
            for (int ligne = zone[num_zone].x; ligne <= zone[num_zone].x1; ligne++)
            {
                for(int colone = zone[num_zone].y; colone <= zone[num_zone].y1; colone++)
                {
                    if ((grille_tirs[colone][ligne] == '.') && ((ligne % 2 == 0 && colone % 2 != 0) || (ligne % 2 != 0 && colone % 2 == 0)))
                    {
                        trouve = true;
                        break;
                    }                    
                }
                if (trouve == true)
                {
                    break;
                }
            }
            if (trouve == false)
            {
                printf("pastrouve:%d %d %d %d %d/ ",num_zone,zone[num_zone].x,zone[num_zone].x1,zone[num_zone].y,zone[num_zone].y1);
                zone[num_zone].tir_libre = false;
            }
            
        }
    }
    
}


void restrain_choix(int flotte_estime[8][4],bool restrain[8],int num_zone,int navire,int *nbr_navire,int *nbr_restrain)
{
    if(flotte_estime[num_zone][navire] == *nbr_navire && restrain[num_zone] == true)
    {
        return;

    }
    else if (flotte_estime[num_zone][navire] < *nbr_navire && restrain[num_zone] == true)
    {
        restrain[num_zone] = false;
        *nbr_restrain = *nbr_restrain + 1;
        printf("restrain:%d %d/ ",num_zone,*nbr_restrain);
        return;
    }
    else if (flotte_estime[num_zone][navire] > *nbr_navire && restrain[num_zone] == true)
    {
        *nbr_navire = flotte_estime[num_zone][navire];
        for (int posterieur = num_zone-1; posterieur >= 0; posterieur--)
        {
            if(restrain[posterieur] == true)
            {
                restrain[posterieur] = false;
                *nbr_restrain = *nbr_restrain + 1;
                printf("restrain:%d %d/ ",posterieur,*nbr_restrain);
            }
        } 
        return;
    }

}


void choisi_zone(bool restrain_x[8],bool restrain_y[8],int *choix_zone_x,int *choix_zone_y)
{
    for (int num_zone = 0; num_zone < 8; num_zone++)
    {
        if (restrain_x[num_zone] == true && restrain_y[num_zone] == true)
        {
            *choix_zone_x = num_zone;
            *choix_zone_y = num_zone;
            return;
        }
        else  
        {
            if (restrain_x[num_zone] == true)
            {
                *choix_zone_x = num_zone;
            }
            if (restrain_y[num_zone] == true)
            {
                *choix_zone_y = num_zone;
            }
        }       
    }
    
}


void estime_flotte_par_zone(Navire navires[5],zone zone,char grille_tirs[10][10],int flotte_estime_x[4],int flotte_estime_y[4])
{
    int longueur,compteur,nombre_navire;
    for (int i = 0; i < 4; i++)
    {
        flotte_estime_x[i] = 0;
        flotte_estime_y[i] = 0;
    }

    for (int num_navire = 0; num_navire < 5; num_navire++)
    {
        if((navires[num_navire].etat == true) && !(num_navire == 3 && navires[3].etat == true && navires[4].etat == true))
        {
            estime_zone(navires[num_navire],zone,grille_tirs,flotte_estime_y,true);
            estime_zone(navires[num_navire],zone,grille_tirs,flotte_estime_x,false);
        }


    }

}


void estime_direction_cible(zone *zone,int flotte_estime_x[4],int flotte_estime_y[4])
{
    for (int navire = 0; navire < 4; navire++)
    {
        if ((flotte_estime_x[navire] > flotte_estime_y[navire]) || (flotte_estime_x[navire] == flotte_estime_y[navire] && flotte_estime_x[navire] != 0 && navire == 3))
        {
            printf("Xchoisi%d %d %d %d/ ",zone->taille_cible[0],zone->taille_cible[1],flotte_estime_x[navire],flotte_estime_y[navire]);
            zone->taille_cible[1] = 0;
            return;
        }
        else if (flotte_estime_x[navire] < flotte_estime_y[navire])
        {
            printf("Ychoisi:%d %d %d %d/ ",zone->taille_cible[0],zone->taille_cible[1],flotte_estime_x[navire],flotte_estime_y[navire]);
            zone->taille_cible[0] = 0;
            return;
        }
    } 
}


int estime_taile_navire_cible(int flotte_estime[4])
{
    if (flotte_estime[0] >= 1)
    {
        return 5;
    }
    else if (flotte_estime[1] >= 1)
    {
        return 4;
    }
    else if (flotte_estime[2] >= 1)
    {
        return 3;
    }
    else if (flotte_estime[3] >= 1)
    {
        return 2;
    }
    else
    {
        return 0;
    }
}


void estime_zone(Navire navires,zone zone,char grille_tirs[10][10],int flotte_estime[4],bool ligne_premier)
{
    int a1,a2,b1,b2,ligne,colone,compteur,nombre_navire = 0;
    definir_ligne_colone(navires,zone,ligne_premier,&a1,&a2,&b1,&b2);
    for (int a = a1; a <= a2; a++)
    {
        compteur = 0;
        for (int b = b1; b <= b2; b++)
        {
            if (ligne_premier == true)
            {
                ligne = a;
                colone = b;      
            }
            else
            {
                ligne = b;
                colone = a;
            }
            update_compteur(navires,zone,grille_tirs,colone,ligne,&compteur,&nombre_navire,ligne_premier);
            
        }
    }
    remplis_estime_navires(navires,flotte_estime,nombre_navire);

    if ((ligne_premier == true && zone.y == zone.y1) || (ligne_premier == false && zone.x == zone.x1))
    {
        for (int i = 0; i < 3; i++)
        {
            flotte_estime[i] = 0; 
        }
    }
}


void definir_ligne_colone(Navire navires,zone zone,bool ligne_premier,int *a1,int *a2,int *b1,int *b2)
{
    int temp;
    if (ligne_premier == true)
    {
        //printf("true ");
        *a1 = zone.x;
        *a2 = zone.x1;
        *b1 = zone.y;
        *b2 = zone.y1;
    }
    else
    {
        *b1 = zone.x;
        *b2 = zone.x1;
        *a1 = zone.y;
        *a2 = zone.y1;
        //printf("false ");
    }
    //printf("a,b:%d %d %d %d/ ",*a1,*a2,*b1,*b2);

    if (*b1 != 0 && *b1 != 9)
    {
     adapter_au_bord(navires,&*b1);
    }
     adapter_au_bord(navires,&*b2);
     if (*b1 > *b2)
     {
        temp = *b1;
        *b1 = *b2;
        *b2 = temp;
     }
     //printf("bis a,b:%d %d %d %d/ ",*a1,*a2,*b1,*b2);
}


void adapter_au_bord(Navire navires,int *x)
{
    if (*x == 1 )
    {
        *x = *x - 1;
    }
    else if (*x == 8)
    {
        *x = *x + 1;
    }
    else if (*x == 4 || *x == 0)
    {
        *x = *x + navires.longueur - 1;
    }
    else if (*x == 5 || *x == 9)
    {
        *x = *x - navires.longueur + 1;
    }
}


void remplis_estime_navires(Navire navires,int flotte_estime[4],int nombre_navire)
{
    if(navires.longueur == 5)
    {
        flotte_estime[0] = flotte_estime[0] + nombre_navire;
    }
    else if (navires.longueur == 4)
    {
        flotte_estime[1] =  flotte_estime[1] + nombre_navire;
    }
    else if (navires.longueur == 3)
    {
        flotte_estime[2] = flotte_estime[2] + nombre_navire;
    }
    else if (navires.longueur == 2)
    {
        flotte_estime[3] = flotte_estime[3] + nombre_navire;
    }

}


void update_compteur(Navire navires,zone zone,char grille_tirs[10][10],int colone,int ligne,int *compteur,int *nombre_navire,bool ligne_premier)
{
    int longueur;
    longueur = navires.longueur;

    if (grille_tirs[colone][ligne]=='.')
    {
        *compteur = *compteur + 1;
    }
    else
    {
        *compteur = 0;
    }

    if (*compteur == longueur)
    {
        *nombre_navire = *nombre_navire + 1;
        *compteur = 0;
    }

}
