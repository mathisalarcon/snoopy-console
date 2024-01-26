#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <unistd.h>

#include "niveau.h"
#include "../menu/menu.h"
#include "../tools/tools.h"

int niveau_existe(char *niveau) { // Vérifie si le niveau existe
    char path[100]; // On prépare la mémoire pour stocker le chemin du fichier
    sprintf(path, "../assets/niveaux/%s.txt", niveau); // On stocke le chemin du fichier dans la variable path
    return access(path, F_OK) != -1; // On retourne si le fichier existe ou non
}

int affiche_unite(int indice, int unites_rouges) { // Affiche une unité du chrono
    if(indice < unites_rouges) { // Si l'unité est rouge
        COULEUR(ROUGE_FONCE, NOIR); // On met la couleur en rouge
        wprintf(L"█"); // On affiche un carré plein
        COULEUR(TURQUOISE, NOIR); // On remet la couleur par défaut
        return 1; // On renvoie que l'unité est rouge
    } else { // Si l'unité est verte
        COULEUR(VERT_FONCE, NOIR); // On met la couleur en vert
        wprintf(L"█"); // On affiche un carré plein
        COULEUR(TURQUOISE, NOIR); // On remet la couleur par défaut
        return 0; // On renvoie que l'unité est verte
    }
}

struct Dimensions dimensions_niveau(int niveau) { // Récupère les dimensions du niveau
    char path[100]; // On prépare la mémoire pour stocker le chemin du fichier
    sprintf(path, "../assets/niveaux/%d.txt", niveau); // On stocke le chemin du fichier dans la variable path

    FILE *fichier = fopen(path, "r"); // On ouvre le fichier
    if(fichier == NULL) { // Si le fichier n'existe pas
        perror("Impossible d'ouvrir le fichier"); // On affiche une erreur
    }

    char ligne[100]; // On prépare la mémoire pour stocker une ligne du fichier

    fgets(ligne, sizeof ligne, fichier); // On récupère la première ligne du fichier

    struct Dimensions dimensions; // On prépare la mémoire pour stocker les dimensions du niveau
                                // La taille de la chaine de caractère -1 (caractère '\n') correspond à la largeur du niveau

    dimensions.largeur = strlen(ligne)-1; // On stocke la largeur du niveau
    dimensions.hauteur = 1; // On initialise la hauteur du niveau à 1 (car on a déjà récupéré la première ligne)

    while(fgets(ligne, sizeof ligne, fichier) != NULL) { // On récupère chaque ligne du fichier
        dimensions.hauteur++; // On incrémente la hauteur du niveau (car chaque ligne correspond à une ligne du niveau)
    }

    return dimensions; // On retourne les dimensions du niveau
}

struct ModeleNiveau modele_niveau(char *id, int sauvegarde) { // Récupère le modèle du niveau
    char path[100]; // On prépare la mémoire pour stocker le chemin du fichier
    if(sauvegarde) { // Si on charge une sauvegarde
        sprintf(path, "../assets/sauvegardes/%s.txt", id); // On stocke le chemin du fichier dans la variable path
    } else { // Si on charge un niveau
        sprintf(path, "../assets/niveaux/%s.txt", id); // On stocke le chemin du fichier dans la variable path
        if(!niveau_existe(id)) { // Si le niveau n'existe pas
            system("cls"); // Efface le terminal
            wprintf(L"Votre partie est terminee !"); // On termine la partie
            sleep(3); // On attend 3 secondes
            system("cls"); // Efface le terminal
            afficher_fichier("../assets/ASCII/logo.txt"); // On affiche le logo
            menu(); // On relance le menu
        }
    }

    struct Coordonnees snoopy = {0, 0}; // On prépare la mémoire pour stocker les coordonnées de Snoopy
    struct Coordonnees *oiseaux = malloc(4 * sizeof(struct Coordonnees)); // On prépare la mémoire pour stocker les coordonnées des oiseaux

    int temps_restant = 0; // On initialise le temps restant à 0
    int niveau = atoi(id); // On convertit l'id du niveau en entier
    int nb_vies = 3; // On initialise le nombre de vies à 3
    int score = 0; // On initialise le score à 0
    int sous_case = 0; // On initialise la sous case à 0

    int nb_oiseaux = 0; // On initialise le nombre d'oiseaux à 0
    int nb_teleporteurs = 0; // On initialise le nombre de téléporteurs à 0
    int nb_balles = 0; // On initialise le nombre de balles à 0


    struct Balle *balles = malloc(1 * sizeof(struct Balle)); // On prépare la mémoire pour stocker les balles

    FILE *fichier = fopen(path, "r"); // On ouvre le fichier
    if(fichier == NULL) // Si le fichier n'existe pas
        perror("Impossible d'ouvrir le fichier"); // On affiche une erreur

    if(sauvegarde) { // Si on charge une sauvegarde
        // TEMPS\n
        // NIVEAU\n
        // NB_VIES\n
        // SCORE\n
        // SOUS_CASE\n
        // NB_BALLES\n
        // X\n
        // Y\n
        // DIRECTION\n
        // ...

        char ligne[100]; // On prépare la mémoire pour stocker une ligne du fichier
        fgets(ligne, sizeof ligne, fichier); // On récupère la première ligne du fichier
        temps_restant = atoi(ligne); // On stocke le temps restant

        fgets(ligne, sizeof ligne, fichier); // On récupère la deuxième ligne du fichier
        niveau = atoi(ligne); // On stocke le niveau

        fgets(ligne, sizeof ligne, fichier); // On récupère la troisième ligne du fichier
        nb_vies = atoi(ligne); // On stocke le nombre de vies

        fgets(ligne, sizeof ligne, fichier); // On récupère la quatrième ligne du fichier
        score = atoi(ligne); // On stocke le score

        fgets(ligne, sizeof ligne, fichier); // On récupère la cinquième ligne du fichier
        sous_case = atoi(ligne); // On stocke la sous case

        fgets(ligne, sizeof ligne, fichier); // On récupère la sixième ligne du fichier
        nb_balles = atoi(ligne); // On stocke le nombre de balles

        balles = malloc(nb_balles * sizeof(struct Balle)); // On prépare la mémoire pour stocker les balles
        for(int i = 0; i < nb_balles; i++) { // On parcourt les balles
            struct Balle balle = {0, 0, 0}; // On prépare la mémoire pour stocker les coordonnées et la direction de la balle
            fgets(ligne, sizeof ligne, fichier); // On récupère la ligne du fichier
            balle.x = atoi(ligne); // On stocke la position x de la balle
            fgets(ligne, sizeof ligne, fichier); // On récupère la ligne du fichier
            balle.y = atoi(ligne); // On stocke la position y de la balle
            fgets(ligne, sizeof ligne, fichier); // On récupère la ligne du fichier
            balle.direction = atoi(ligne); // On stocke la direction de la balle

            balles[i] = balle; // On stocke la balle dans le tableau de balles

            // On affiche les valeurs
            wprintf(L"(%d) x: %d, y: %d, direction: %d\n", i, balle.x, balle.y, balle.direction);
        }
    } else id = (char *)time(NULL); // Si on charge un niveau, on stocke l'id de la partie en cours


    struct Dimensions dimensions = dimensions_niveau(niveau); // On récupère les dimensions du niveau

    int **modele = malloc(dimensions.hauteur * sizeof(int *)); // On prépare la mémoire pour stocker le modèle du niveau

    int nb_balles2 = 0;
    for(int i = 0; i < dimensions.hauteur; i++) { // On parcourt les lignes du fichier
        modele[i] = malloc(dimensions.largeur * sizeof(int)); // On prépare la mémoire pour stocker les colonnes du modèle
        for(int j = 0; j < dimensions.largeur; j++) { // On parcourt les colonnes du fichier
            char c = fgetc(fichier); // On récupère le caractère
            switch (c) { // On stocke le caractère dans le modèle
                case '\n': // Si le caractère est un retour à la ligne
                    c = fgetc(fichier); // On récupère le caractère suivant
                    break;
                case '5': // Si le caractère est un téléporteur
                    nb_teleporteurs++; // On incrémente le nombre de téléporteurs
                    break;
                case '7': // Si le caractère est la balle
                    nb_balles2++;
                    break;
                case '8': // Si le caractère est Snoopy
                    snoopy.x = j; // On stocke la position x de Snoopy
                    snoopy.y = i; // On stocke la position y de Snoopy
                    break;
                case '9': // Si le caractère est un oiseau
                    oiseaux[nb_oiseaux].x = j; // On stocke la position x de l'oiseau
                    oiseaux[nb_oiseaux].y = i; // On stocke la position y de l'oiseau
                    nb_oiseaux++; // On incrémente le nombre d'oiseaux
                    break;
            }
            modele[i][j] = (c-'0'); // On stocke le caractère dans le modèle
        }
    }

    if(!sauvegarde) balles = malloc(nb_balles2 * sizeof(struct Balle)); // On prépare la mémoire pour stocker les balles

    struct Coordonnees *teleporteurs = malloc(nb_teleporteurs * sizeof(struct Coordonnees)); // On prépare la mémoire pour stocker les coordonnées des téléporteurs

    int nb_teleporteurs2 = 0; // On initialise le nombre de téléporteurs à 0
    nb_balles2 = 0; // On réinitialise le nombre de balles à 0
    for(int i = 1; i < dimensions.hauteur-1; i++) // On parcourt les lignes du modèle
        for(int j = 1; j < dimensions.largeur-1; j++) { // On parcourt les colonnes du modèle
            if (modele[i][j] == 5)
                teleporteurs[nb_teleporteurs2++] = (struct Coordonnees) {j, i}; // On stocke les coordonnées des téléporteurs
            if (modele[i][j] == 7) {
                struct Balle balle = {0, 0, rand() % 4}; // On prépare la mémoire pour stocker les coordonnées et la direction de la balle
                balle.x = j; // On stocke la position x de la balle
                balle.y = i; // On stocke la position y de la balle
                balles[nb_balles2++] = balle; // On stocke la balle dans le tableau de balles
                modele[i][j] = 0;
            }
        }
    char message[500]; // On prépare la mémoire pour stocker le message

    struct ModeleNiveau result = { // On retourne le modèle du niveau
            id,
            modele,
            dimensions.hauteur,
            dimensions.largeur,
            niveau,
            temps_restant,
            nb_vies,
            nb_oiseaux,
            nb_teleporteurs,
            nb_balles | nb_balles2,
            message,
            score,
            sous_case,
            snoopy,
            oiseaux,
            teleporteurs,
            balles
    };
    return result;
}

void afficher_niveau(struct ModeleNiveau modele, int temps_restant, char derniere_direction) { // Affiche le niveau

    // Calcul du temps restant en pourcentage (pourcentage de la barre verte)
    float temps_restant_prct = (float)(temps_restant * 100) / (float)120;
    // Calcul du nombre d'unités rouges
    int unites_rouges = (modele.hauteur-1)*2+(modele.largeur-1)*2 - unite_chrono_restant(temps_restant_prct, modele.hauteur, modele.largeur);
    int indice_unite = 0; // On initialise l'indice de l'unité à 0

    for(int k = 0; k < modele.largeur; k++) { // On affiche la première ligne du chrono
        int z = affiche_unite(indice_unite, unites_rouges); // On affiche une unité du chrono (rouge ou verte)
        if(z) indice_unite++; // Si l'unité est rouge, on incrémente l'indice de l'unité
    }
    wprintf(L"\n"); // On affiche un retour à la ligne



    for(int i = 1; i < modele.hauteur-1; i++) { // On parcourt les lignes du modèle
        for(int j = 0; j < modele.largeur; j++) { // On parcourt les colonnes du modèle
            if(j == 0) { // Si on est sur la première colonne (donc le chrono)
                affiche_unite(2*(modele.hauteur + modele.largeur - 2) - i, unites_rouges); // On affiche une unité du chrono (rouge ou verte)
            } else if(j == modele.largeur-1) { // Si on est sur la dernière colonne (donc le chrono)
                int z = affiche_unite(indice_unite, unites_rouges); // On affiche une unité du chrono (rouge ou verte)
                if(z) indice_unite++; // Si l'unité est rouge, on incrémente l'indice de l'unité
            } else { // Si on est sur une autre colonne (donc le contenu du niveau)
                int est_sur_balle = 0;
                for(int k=0; k < modele.nb_balles; k++) {
                    if(modele.balles[k].x == j && modele.balles[k].y == i) {
                        est_sur_balle = 1;
                        COULEUR(BLANC, NOIR); // On met la couleur en blanc
                        wprintf(L"●"); // On affiche la balle
                        COULEUR(TURQUOISE, NOIR); // On remet la couleur par défaut
                        break;
                    }
                }
                if(!est_sur_balle) switch(modele.modele[i][j]) { // On affiche le contenu de la case
                    case 0: // Si la case est vide
                        wprintf(L" ");
                        break;
                    case 1: // Si la case est un bloc cassable
                        wprintf(L"▨");
                        break;
                    case 2: // Si la case est un bloc poussable
                        wprintf(L"▢");
                        break;
                    case 3: // Si la case est un bloc piégé
                        wprintf(L"▨");
                        break;
                    case 4: // Si la case est un bloc indestructible
                        wprintf(L"█");
                        break;
                    case 5: // Si la case est un téléporteur
                        wprintf(L"⏏");
                        break;
                    case 6: // Si la case est un tapis roulant
                        wprintf(L"☰");
                        break;
                    case 8: // Si la case est Snoopy
                        COULEUR(JAUNE, NOIR); // On met la couleur en jaune
                        switch (derniere_direction) { // On affiche Snoopy en fonction de sa dernière direction
                            case 'z': // Si la dernière direction est le haut
                                wprintf(L"▲");
                                break;
                            case 'q': // Si la dernière direction est la gauche
                                wprintf(L"◀");
                                break;
                            case 's': // Si la dernière direction est le bas
                                wprintf(L"▼");
                                break;
                            case 'd': // Si la dernière direction est la droite
                                wprintf(L"▶");
                                break;
                            default: // Si la dernière direction est inconnue
                                wprintf(L"X");
                                break;
                        }
                        COULEUR(TURQUOISE, NOIR); // On remet la couleur par défaut
                        break;
                    case 9: // Si la case est un oiseau
                        COULEUR(VIOLET, NOIR); // On met la couleur en violet
                        wprintf(L"★");
                        COULEUR(TURQUOISE, NOIR); // On remet la couleur par défaut
                        break;
                }
            }
        }
        wprintf(L"\n");
    }
    for(int k = 0; k<modele.largeur; k++) {
        int z = affiche_unite(indice_unite+modele.largeur-k, unites_rouges);
        if(z) indice_unite++;
    }
    wprintf(L"\n");
}

int unite_chrono_restant(int prct_restant, int hauteur, int largeur) { // Récupère le nombre d'unités rouges du chrono
    int unites = (largeur-1)*2 + (hauteur-1)*2; // On calcule le nombre d'unités du chrono
    return (unites * prct_restant) / 100; // On retourne le nombre d'unités rouges
}

int nombre_oiseaux(struct ModeleNiveau modele) { // Récupère le nombre d'oiseaux restants
    int nb_oiseaux = 0; // On initialise le nombre d'oiseaux à 0
    for(int i = 0; i < modele.nb_oiseaux; i++) { // On parcourt les oiseaux
        if(modele.oiseaux[i].x != modele.largeur && modele.oiseaux[i].y != modele.hauteur) nb_oiseaux++; // Si l'oiseau n'est pas mort, on incrémente le nombre d'oiseaux
    }

    return nb_oiseaux;
}