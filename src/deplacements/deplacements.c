#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>

#include "deplacements.h"
#include "commandes.h"
#include "../niveau/niveau.h"
#include "../tools/tools.h"

int pousse_bloc(struct ModeleNiveau *modele, int x, int y, int direction);
void teleportation(struct ModeleNiveau *modele, int x, int y);
void oiseau(struct ModeleNiveau *modele);
void tapis_roulant(struct ModeleNiveau *modele, int x, int y, int direction);

int getCaseValue(struct ModeleNiveau modele, int x, int y) {
    return modele.modele[y][x]; // On inverse x et y car le tableau est en [y][x]
}

void deplacer(struct ModeleNiveau *modele, char direction, char *derniere_direction) { // Fonction de deplacement de snoopy
    *derniere_direction = direction; // On enregistre la derniere direction (pour casser les blocs)
    int add_x = 0; // On initialise les variables d'ajout de coordonnees
    int add_y = 0;
    switch (direction) { // On ajoute les coordonnees en fonction de la direction
        case 'z': // Nord
            add_y = -1; // On ajoute -1 a y pour monter
            break;
        case 'q': // Ouest
            add_x = -1; // On ajoute -1 a x pour aller a gauche
            break;
        case 's': // Sud
            add_y = 1; // On ajoute 1 a y pour descendre
            break;
        case 'd': // Est
            add_x = 1; // On ajoute 1 a x pour aller a droite
            break;
    }

    // On stock les coordonnees de l'ancien snoopy pour pouvoir savoir si la balle a touche snoopy
    struct Coordonnees ancien_snoopy = (struct Coordonnees) { modele->snoopy.x, modele->snoopy.y };

    // On recupere la valeur de la case ou on veut aller
    int nouvelle_case = getCaseValue(*modele, modele->snoopy.x+add_x, modele->snoopy.y+add_y);

    if(nouvelle_case == 0 || nouvelle_case == 2 && pousse_bloc(modele, modele->snoopy.x+add_x, modele->snoopy.y+add_y, direction)) { // Case vide ou bloc poussable
        modele->modele[modele->snoopy.y][modele->snoopy.x] = modele->sous_case; // On remet l'état de la case sur laquelle était snoopy
        modele->sous_case = 0; // On remet l'état de la case sous snoopy a 0 (case vide)
        modele->snoopy.y += add_y; // On ajoute les coordonnees
        modele->snoopy.x += add_x;
        modele->sous_case = modele->modele[modele->snoopy.y][modele->snoopy.x]; // On stock l'état de la case sous snoopy
        modele->modele[modele->snoopy.y][modele->snoopy.x] = 8; // On met snoopy sur la case
    } else if(nouvelle_case == 5) { // Bloc teleportation
        modele->modele[modele->snoopy.y][modele->snoopy.x] = modele->sous_case; // On remet l'état de la case sur laquelle était snoopy
        teleportation(modele, modele->snoopy.x+add_x, modele->snoopy.y+add_y); // On teleporte snoopy
    } else if(nouvelle_case == 6) { // Tapis roulant
        modele->modele[modele->snoopy.y][modele->snoopy.x] = modele->sous_case; // On remet l'état de la case sur laquelle était snoopy
        tapis_roulant(modele, modele->snoopy.x+add_x, modele->snoopy.y+add_y, direction); // On deplace snoopy
    } else if(nouvelle_case == 9) { // Oiseau
        modele->modele[modele->snoopy.y][modele->snoopy.x] = modele->sous_case; // On remet l'état de la case sur laquelle était snoopy
        modele->sous_case = 0; // On remet l'état de la case sous snoopy a 0 (case vide)
        modele->snoopy.y += add_y; // On ajoute les coordonnees
        modele->snoopy.x += add_x;
        modele->modele[modele->snoopy.y][modele->snoopy.x] = 8; // On met snoopy sur la case
        oiseau(modele); // On fait disparaitre l'oiseau
    }
    // Pour toutes les balles
    for(int i=0; i < modele->nb_balles; i++) {
        struct Balle balle = modele->balles[i]; // On recupere la balle
        if((modele->snoopy.x != ancien_snoopy.x || modele->snoopy.y != ancien_snoopy.y) && balle.x == modele->snoopy.x && balle.y == modele->snoopy.y) { // Si la balle a touche snoopy
            modele->vies_restantes--; // On enleve une vie
            modele->message = "La balle vous a touche, vous avez perdu une vie !"; // On initialise le message que le code affichera dans la boucle principale
        }
    }
}
void deplacer_balle(struct ModeleNiveau *modele, int i) { // Fonction de deplacement de la balle
    int add_x = 0; // On initialise les variables d'ajout de coordonnees
    int add_y = 0;
    if(modele->balles[i].y == modele->hauteur-2 && modele->balles[i].direction <= 1) { // Si la balle est en bas et qu'elle va vers le bas (traitement Nord/Sud)
        modele->balles[i].direction = modele->balles[i].direction == 0 ? 3 : 2; // Sud Est -> Nord Est, Sud Ouest -> Nord Ouest
        return deplacer_balle(modele, i); // On rappelle la fonction pour deplacer la balle pour retraiter son état et changer sa direciton (est/ouest)
    }
    if(modele->balles[i].y == 1 && modele->balles[i].direction > 1) { // Si la balle est en haut et qu'elle va vers le haut (traitement Nord/Sud)
        modele->balles[i].direction = modele->balles[i].direction == 2 ? 1 : 0; // Nord Ouest -> Sud Ouest, Nord Est -> Sud Est
        return deplacer_balle(modele, i); // On rappelle la fonction pour deplacer la balle pour retraiter son état et changer sa direciton (est/ouest)
    }
    if(modele->balles[i].x == modele->largeur-2 && modele->balles[i].direction%3 == 0) { // Si la balle est a droite et qu'elle va vers la droite (traitement Est/Ouest)
        modele->balles[i].direction = modele->balles[i].direction == 3 ? 2 : 1; // Sud Est -> Sud Ouest, Nord Est -> Nord Ouest
        // Pas besoin de rappeler la fonction car la direction Nord/Sud a deja ete traitee précédemment
    }
    if(modele->balles[i].x == 1  && modele->balles[i].direction%3 != 0) { // Si la balle est a gauche et qu'elle va vers la gauche (traitement Est/Ouest)
        modele->balles[i].direction = modele->balles[i].direction == 1 ? 0 : 3; // Sud Ouest -> Sud Est, Nord Ouest -> Nord Est
        // Pas besoin de rappeler la fonction car la direction Nord/Sud a deja ete traitee précédemment
    }
    switch (modele->balles[i].direction) { // On ajoute les coordonnees en fonction de la direction
        case 0: // Sud Est
            add_y = 1; // On ajoute 1 a y pour descendre
            add_x = 1; // On ajoute 1 a x pour aller a droite
            break;
        case 1: // Sud Ouest
            add_y = 1; // On ajoute 1 a y pour descendre
            add_x = -1; // On ajoute -1 a x pour aller a gauche
            break;
        case 2: // Nord Ouest
            add_y = -1; // On ajoute -1 a y pour monter
            add_x = -1; // On ajoute -1 a x pour aller a gauche
            break;
        case 3: // Nord Est
            add_y = -1; // On ajoute -1 a y pour monter
            add_x = 1; // On ajoute 1 a x pour aller a droite
            break;
    }
    modele->balles[i].y += add_y; // On ajoute les coordonnees
    modele->balles[i].x += add_x;
}

void casse_bloc(struct ModeleNiveau *modele, const char *direction) { // Fonction de casse de bloc
    int add_x = 0; // On initialise les variables d'ajout de coordonnees du bloc a casser
    int add_y = 0;
    switch (*direction) {
        case 'z': // Nord
            add_y = -1; // On ajoute -1 a y pour monter
            break;
        case 'q': // Ouest
            add_x = -1; // On ajoute -1 a x pour aller a gauche
            break;
        case 's': // Sud
            add_y = 1; // On ajoute 1 a y pour descendre
            break;
        case 'd': // Est
            add_x = 1; // On ajoute 1 a x pour aller a droite
            break;
    }
    // On recupere la valeur de la case ou on veut aller
    int nouvelle_case = getCaseValue(*modele, modele->snoopy.x+add_x, modele->snoopy.y+add_y);
    if(nouvelle_case == 1 || nouvelle_case == 3) modele->modele[modele->snoopy.y+add_y][modele->snoopy.x+add_x] = 0; // Si la case est un bloc, on la casse
    if(nouvelle_case == 3) { // Si la case est un bloc piege
        modele->vies_restantes--; // On enleve une vie
        modele->message = "Vous avez casse un bloc piege... Vous venez de perdre une vie !"; // On initialise le message que le code affichera dans la boucle principale
    }
}
int pousse_bloc(struct ModeleNiveau *modele, int x, int y, int direction) { // Fonction de pousse de bloc
    int add_x = 0; // On initialise les variables d'ajout de coordonnees du bloc a pousser
    int add_y = 0;
    switch (direction) { // On ajoute les coordonnees en fonction de la direction
        case 'z': // Nord
            add_y = -1; // On ajoute -1 a y pour monter
            break;
        case 'q': // Ouest
            add_x = -1; // On ajoute -1 a x pour aller a gauche
            break;
        case 's': // Sud
            add_y = 1; // On ajoute 1 a y pour descendre
            break;
        case 'd': // Est
            add_x = 1; // On ajoute 1 a x pour aller a droite
            break;
    }

    int nouvelle_case = getCaseValue(*modele, x+add_x, y+add_y); // On recupere la valeur de la case ou on veut aller
    // Si la case est vide ou un bloc poussable (dans ce cas, on rappelle la fonction pour pousser le bloc après le bloc actuel, etc)
    if(nouvelle_case == 0 || (nouvelle_case == 2 && pousse_bloc(modele, x+add_x, y+add_y, direction))) {
        modele->modele[y+add_y][x+add_x] = 4; // On met le bloc sur la case
        modele->modele[y][x] = 0; // On met la case actuelle vide
        return 1; // On retourne 1 pour dire que le bloc a bien ete pousse
    }

    return 0; // On retourne 0 pour dire que le bloc n'a pas ete pousse
}
void teleportation(struct ModeleNiveau *modele, int x, int y) { // Fonction de teleportation de snoopy
    modele->sous_case = 5;
    if(modele->nb_teleporteurs <= 1) return; // Si il n'y a pas d'autre teleporteur, on ne fait rien
    srand(time(NULL)); // On initialise le random
    struct Coordonnees tp_coord; // On initialise les coordonnees du nouveau teleporteur
    do { // On boucle les coordonnees du nouveau teleporteur tant qu'elles sont les memes que celles du tp actuel (pour ne pas se teleporter sur le meme)
        tp_coord = modele->teleporteurs[rand() % modele->nb_teleporteurs]; // On recupere les coordonnees d'un teleporteur aleatoire
    } while (tp_coord.x == x && tp_coord.y == y); // tant qu'elles sont les memes que celles du tp actuel
    modele->snoopy.x = tp_coord.x; // On teleporte snoopy
    modele->snoopy.y = tp_coord.y;
    modele->modele[modele->snoopy.y][modele->snoopy.x] = 8; // On met snoopy sur la case
}
void oiseau(struct ModeleNiveau *modele) { // Fonction de disparition de l'oiseau
    for(int i = 0; i < modele->nb_oiseaux; i++) { // On boucle les oiseaux
        struct Coordonnees oiseau = modele->oiseaux[i]; // On recupere les coordonnees de l'oiseau
        if(oiseau.x == modele->snoopy.x && oiseau.y == modele->snoopy.y) { // Si l'oiseau est sur la case de snoopy
            modele->oiseaux[i].x = modele->largeur; // On le fait disparaitre (on le met en dehors de la map)
            modele->oiseaux[i].y = modele->hauteur;
        }
    }
}
void tapis_roulant(struct ModeleNiveau *modele, int x, int y, int direction) { // Fonction de deplacement de snoopy sur un tapis roulant
    modele->modele[y][x] = 8; // On remet l'état de la case sur laquelle était snoopy
    modele->snoopy.x = x; // On ajoute les coordonnees
    modele->snoopy.y = y;
    modele->sous_case = 6; // On remet l'état de la case sous snoopy a 0 (case vide)

    int add_x = 0; // On initialise les variables d'ajout de coordonnees
    int add_y = 0;
    switch (direction) { // On ajoute les coordonnees en fonction de la direction
        case 'z': // Nord
            add_y = -1; // On ajoute -1 a y pour monter
            break;
        case 'q': // Ouest
            add_x = -1; // On ajoute -1 a x pour aller a gauche
            break;
        case 's': // Sud
            add_y = 1; // On ajoute 1 a y pour descendre
            break;
        case 'd': // Est
            add_x = 1; // On ajoute 1 a x pour aller a droite
            break;
    }

    // On recupere la valeur de la case ou on veut aller
    int nouvelle_case = getCaseValue(*modele, x+add_x, y+add_y);

    // Case vide ou bloc poussable (dans ce cas, on rappelle la fonction pour pousser le bloc après le bloc actuel, etc)
    if(nouvelle_case == 0 || (nouvelle_case == 2 && pousse_bloc(modele, x+add_x, y+add_y, direction))) {
        modele->modele[modele->snoopy.y][modele->snoopy.x] = modele->sous_case; // On remet l'état de la case sur laquelle était snoopy
        modele->sous_case = 0; // On remet l'état de la case sous snoopy a 0 (case vide)
        modele->snoopy.y = y+add_y; // On ajoute les coordonnees
        modele->snoopy.x = x+add_x;
        modele->modele[modele->snoopy.y][modele->snoopy.x] = 8; // On met snoopy sur la case
    } else if(nouvelle_case == 5) { // Bloc apparition/disparition
        modele->modele[modele->snoopy.y][modele->snoopy.x] = modele->sous_case; // On remet l'état de la case sur laquelle était snoopy
        teleportation(modele, x+add_x, y+add_y); // On teleporte snoopy
    } else if(nouvelle_case == 6) { // Tapis roulant
        modele->modele[modele->snoopy.y][modele->snoopy.x] = modele->sous_case; // On remet l'état de la case sur laquelle était snoopy
        tapis_roulant(modele, x+add_x, y+add_y, direction); // On deplace snoopy
    } else if(nouvelle_case == 9) { // Oiseau
        modele->modele[modele->snoopy.y][modele->snoopy.x] = modele->sous_case; // On remet l'état de la case sur laquelle était snoopy
        modele->snoopy.y = y+add_y; // On ajoute les coordonnees
        modele->snoopy.x = x+add_x;
        modele->modele[modele->snoopy.y][modele->snoopy.x] = 8; // On met snoopy sur la case
        modele->sous_case = 0; // On remet l'état de la case sous snoopy a 0 (case vide)
        oiseau(modele); // On fait disparaitre l'oiseau
    } else { // Si la case est un bloc sur lequel snoopy ne peut pas aller (bloc piege, bloc indestructible, etc)
        modele->snoopy.y = y; // On ajoute les coordonnees
        modele->snoopy.x = x;
        modele->modele[modele->snoopy.y][modele->snoopy.x] = 8; // On met snoopy sur la case
    }
}