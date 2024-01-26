#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <conio.h>

#include "partie.h"
#include "../menu/menu.h"
#include "../niveau/niveau.h"
#include "../tools/tools.h"
#include "../chrono/chrono.h"
#include "../deplacements/commandes.h"
#include "../deplacements/deplacements.h"
#include "../sauvegardes/sauvegardes.h"

void nouvelle_partie(char *id) { // id = nom du fichier de sauvegarde
    jeu(modele_niveau(id, 0)); // 0 = niveau traditionnel, 1 = sauvegarde
}

void jeu(struct ModeleNiveau modele) { // Boucle principale du jeu
    // Affiche le temps restant
    system("cls"); // Clear la console
    int temps_arrivee = modele.temps_restant > 0 ? (int)time(NULL) + modele.temps_restant : nouveau_chrono(); // Temps d'arrivée = temps actuel + 120 secondes
    int pause = 0; // On initialise la pause à 0 (non-pause)
    int menu_principal = 0; // On initialise le menu principal à 0 (non-ouvert)

    char derniere_direction = ' '; // On initialise la dernière direction à ' ' (inconnue)
    // Boucle principale du jeu
    // Tant que:
    // - Le temps d'arrivée est supérieur au temps actuel   OU      la pause est activée
    // - ET Il reste des oiseaux
    // - ET Il reste des vies
    // - ET Le menu principal n'est pas ouvert
    while((temps_arrivee - (int) time(NULL) > 0 || pause != 0) && nombre_oiseaux(modele) && modele.vies_restantes && !menu_principal) {
        handleKeypress(&modele, &temps_arrivee, &pause, &menu_principal, &derniere_direction); // Gestion des touches
        if(pause != 0) continue; // Si la pause est activée, on passe à l'itération suivante
        system("cls"); // Clear la console

        afficher_niveau(modele, temps_arrivee - (int) time(NULL), derniere_direction); // Afficher le niveau

        for(int i=0; i<modele.nb_balles; i++) {
            deplacer_balle(&modele, i); // Déplacer la balle
        }
        if(strlen(modele.message)) { // Si un message doit être affiché
            wprintf(L"Message: \n"); // Afficher le message
            COULEUR(ROUGE_FLUO, NOIR); // Couleur rouge
            wprintf(L"%s\n", modele.message);
            COULEUR(TURQUOISE, NOIR);
        }
        afficher_vies(modele.vies_restantes); // Afficher les vies
        for(int i=0; i<modele.nb_balles; i++) {
            if(modele.balles[i].x == modele.snoopy.x && modele.balles[i].y == modele.snoopy.y && modele.vies_restantes) { // Si la balle touche Snoopy
                modele.vies_restantes--; // On enlève une vie
                modele.message = "La balle vous a touche, vous avez perdu une vie !"; // On affiche un message
            }
        }
        usleep(250000); // On attend 250ms (0.25s)
    }

    if(menu_principal == 1) { // Si le menu principal est ouvert
        system("cls"); // Clear la console
        afficher_fichier("../assets/ASCII/logo.txt"); // Afficher le logo
        menu(); // Afficher le menu
    } else if(!nombre_oiseaux(modele)) { // Si il n'y a plus d'oiseaux
        int temps_restant = temps_arrivee - (int) time(NULL); // Temps restant = temps d'arrivée - temps actuel
        float temps_restant_prct = (float)(temps_restant * 100) / (float)120; // Temps restant en pourcentage
        modele.score += temps_restant*100; // On ajoute le score du niveau au score total
        system("cls"); // Clear la console
        afficher_fichier("../assets/ASCII/victoire.txt"); // Afficher le logo de victoire
        wprintf(L"\n\nScore du niveau %d : %d\n", modele.niveau, temps_restant*100); // Afficher le score du niveau
        wprintf(L"Temps restant : %d (%.2f%%)\n", temps_restant, temps_restant_prct); // Afficher le temps restant + pourcentage (.2f = 2 chiffres après la virgule)
        wprintf(L"Nombre de vies restantes : %d\n", modele.vies_restantes); // Afficher le nombre de vies restantes
        wprintf(L"Score total : %d\n", modele.score); // Afficher le score total
        wprintf(L"Appuyez sur une touche pour continuer...\n");
        sleep(1);
        while(!kbhit()) usleep(250000); // Attendre qu'une touche soit pressée (kbhit = keyboard hit : clavier touché)

        int score = modele.score; // On sauvegarde le score
        int vies = modele.vies_restantes; // On sauvegarde le nombre de vies restantes
        char nouveau_niveau[10]; // On initialise une variable pour le nouveau niveau
        sprintf(nouveau_niveau, "%d", modele.niveau+1); // On convertit le niveau en chaine de caractères

        modele = modele_niveau(nouveau_niveau, 0); // On charge le nouveau niveau
        modele.score = score; // On charge le score
        modele.vies_restantes = vies; // On charge le nombre de vies restantes

        sauvegarder_partie(modele, 120, "../assets/sauvegardes/sauvegarde_temporaire.txt"); // On sauvegarde la partie
        charger_sauvegarde("sauvegarde_temporaire", 0); // On charge la sauvegarde
    } else if((temps_arrivee - (int) time(NULL) <= 0) || !modele.vies_restantes) { // Si le temps d'arrivée est inférieur au temps actuel OU qu'il n'y a plus de vies restantes
        system("cls"); // Clear la console
        afficher_fichier("../assets/ASCII/defaite.txt"); // Afficher le logo de défaite
        if(modele.vies_restantes) modele.vies_restantes--; // Si il reste des vies, on enlève une vie
        if(modele.vies_restantes) wprintf(L"Vous avez été pris par le temps !"); // Si il reste des vies, on affiche un message
        else wprintf(L"Vous n'avez plus de vies restantes !"); // Sinon, on affiche un autre message
        wprintf(L"\n\nNombre de vies restantes : %d\n", modele.vies_restantes);
        wprintf(L"Score total : %d\n", modele.score);

        if(!modele.vies_restantes) { // Si il n'y a plus de vies restantes
            wprintf(L"Appuyez sur une touche pour retourner au menu principal...\n");
            sleep(1);
            while(!kbhit()); // Attendre qu'une touche soit pressée (kbhit = keyboard hit : clavier touché)
            system("cls");
            afficher_fichier("../assets/ASCII/logo.txt"); // Afficher le logo
            menu(); // Afficher le menu
            return;
        }

        wprintf(L"Appuyez sur une touche pour ressayer le niveau...\n");
        sleep(1);
        while(!kbhit()); // Attendre qu'une touche soit pressée (kbhit = keyboard hit : clavier touché)

        int score = modele.score; // On sauvegarde le score
        int vies = modele.vies_restantes; // On sauvegarde le nombre de vies restantes
        char nouveau_niveau[10]; // On initialise une variable pour le nouveau niveau
        sprintf(nouveau_niveau, "%d", modele.niveau); // On convertit le niveau en chaine de caractères
        modele = modele_niveau(nouveau_niveau, 0); // On charge le nouveau niveau
        modele.score = score; // On charge le score
        modele.vies_restantes = vies; // On charge le nombre de vies restantes

        sauvegarder_partie(modele, 120, "../assets/sauvegardes/sauvegarde_temporaire.txt"); // On sauvegarde la partie
        charger_sauvegarde("sauvegarde_temporaire", 0); // On charge la sauvegarde
    }
}

void afficher_vies(int vies) { // Afficher les vies
    if(vies == 0) return; // Si il n'y a plus de vies, on ne fait rien
//    tableau de 3 fichiers
    FILE *fichiers[3]; // On initialise un tableau de 3 fichiers
    for(int i=0; i<vies; i++) fichiers[i] = fopen("../assets/ASCII/coeur.txt", "r"); // On ouvre les fichiers coeur.txt

    char ligne[100]; // On initialise une variable pour la ligne
    int fin = 0; // On initialise la fin à 0 (non-fin)
    while(!fin) { // Tant que la fin n'est pas atteinte
        for(int i=0; i<vies; i++) { // Pour chaque fichier
            if(fgets(ligne, sizeof ligne, fichiers[i]) == NULL) { // Si la ligne est vide
                fin = 1; // On passe la fin à 1 (fin)
                break;
            }
            if(ligne[strlen(ligne)-1] == '\n') ligne[strlen(ligne)-1] = '\0'; // Si la ligne se termine par un retour à la ligne, on le supprime
            for(int j=0; j< strlen(ligne); j++) ligne[j] = ligne[j] == 'x' ? ' ' : ligne[j]; // On remplace les 'x' par des espaces (car les espaces sont supprimés par l'IDE)
            wprintf(L"%s", ligne); // On affiche la ligne
            if(i == vies-1) wprintf(L"\n"); // Si c'est le dernier fichier, on affiche un retour à la ligne
        }
    }

    wprintf(L"\n\n");

    for(int i=0; i<vies; i++) fclose(fichiers[i]); // On ferme les fichiers
}