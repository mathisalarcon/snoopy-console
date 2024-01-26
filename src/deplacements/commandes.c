#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <conio.h>

#include "commandes.h"
#include "../menu/menu.h"
#include "../chrono/chrono.h"
#include "deplacements.h"
#include "../niveau/niveau.h"
#include "../sauvegardes/sauvegardes.h"
#include "../tools/tools.h"

// Lorsque l'utilisateur appuie sur une touche, on regarde quelle touche a été appuyée et on effectue l'action correspondante
void handleKeypress(struct ModeleNiveau *modele, int *temps_arrivee, int *pause, int *menu_principal, char *derniere_direction) {
    if(kbhit()) { // Si une touche a été appuyée (kbhit = keyboard hit : clavier pressé)
        switch(getch()) { // On regarde quelle touche a été appuyée
            case 'p': // Pause
                if(*pause == 0) { // Si le jeu n'est pas déjà en pause
                    *pause = (int) time(NULL); // On enregistre le timestamp de la pause
                    wprintf(L"Pause\n");
                } else { // Si le jeu est déjà en pause
                    *temps_arrivee += (int) time(NULL) - *pause; // On récupère le temps écoulé depuis le début de la pause et on l'ajoute au temps d'arrivée
                    *pause = 0; // On enlève la pause
                }
                break;
            case 's': // Sauvegarder
                ;int pause_timestamp = (int) time(NULL); // On met le jeu en pause
                *menu_principal = 1; // On prévient le jeu qu'on veut revenir au menu principal
                system("cls"); // On efface l'écran

                char *nom = demande_sauvegarde_id(); // On demande à l'utilisateur le nom de la sauvegarde
                char chemin[200]; // On prévoit un espace mémoire pour le chemin de la sauvegarde
                sprintf(chemin, "../assets/sauvegardes/%s.txt", nom); // On crée le chemin de la sauvegarde
                sauvegarder_partie(*modele, *temps_arrivee - pause_timestamp, chemin); // On sauvegarde la partie
                break;
            case 'q': // Abandonner
                if(*pause != 0) break; // Si le jeu est en pause, on ne fait rien
                *menu_principal = 1; // On prévient le jeu qu'on veut revenir au menu principal
                break;
            case 'r': // Recommencer
                if(*pause != 0) break; // Si le jeu est en pause, on ne fait rien
                if(modele->vies_restantes <= 1) { // Si le joueur n'a plus qu'une vie restante
                    system("cls"); // On efface l'écran
                    wprintf(L"Vous n'avez plus qu'une vies restantes ! Vous ne pouvez donc pas réessayer\n"); // On prévient le joueur qu'il ne peut pas recommencer
                    sleep(1); // On attend 1 seconde
                    break;
                }
                *derniere_direction = ' '; // On réinitialise la dernière direction
                int score = modele->score; // On enregistre le score actuel
                int vies = --modele->vies_restantes; // On enlève une vie au joueur
                char niveau[10]; // On prévoit un espace mémoire pour le niveau
                sprintf(niveau, "%d", modele->niveau); // On récupère le niveau actuel
                *modele = modele_niveau(niveau, 0); // On récupère le modèle du niveau actuel
                *temps_arrivee = nouveau_chrono(); // On récupère le temps d'arrivée
                modele->score = score; // On initialise le score au score actuel
                modele->vies_restantes = vies; // On initialise le nombre de vies restantes au nombre de vies restantes actuel
                system("cls"); // On efface l'écran
                afficher_fichier("../assets/ASCII/recommence.txt"); // On affiche le texte de recommencement
                sleep(1); // On attend 1 seconde
                // Le jeu va ensuite recommencer automatiquement
            case 32: // Espace
                if(*pause != 0) break; // Si le jeu est en pause, on ne fait rien
                casse_bloc(modele, derniere_direction); // On casse le bloc se trouvant devant snoopy
                break;
            case 224:
                if(*pause != 0) break; // Si le jeu est en pause, on ne fait rien
                switch (getch()) { // On regarde quelle touche exacte a été appuyée (flèche, laquelle ?)
                    case 72: // Haut
                        deplacer(modele, 'z', derniere_direction); // On déplace snoopy vers le haut
                        break;
                    case 80: // Bas
                        deplacer(modele, 's', derniere_direction); // On déplace snoopy vers le bas
                        break;
                    case 75: // Gauche
                        deplacer(modele, 'q', derniere_direction); // On déplace snoopy vers la gauche
                        break;
                    case 77: // Droite
                        deplacer(modele, 'd', derniere_direction); // On déplace snoopy vers la droite
                        break;
                }
                break;
        }
    }
}