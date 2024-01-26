#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <time.h>
#include <unistd.h>
#include "string.h"
#include "sauvegardes.h"
#include "../niveau/niveau.h"
#include "../chrono/chrono.h"
#include "../menu/menu.h"
#include "../deplacements/commandes.h"
#include "../partie/partie.h"

int sauvegarde_id_valide(char *id) { // Vérifie si l'id de la sauvegarde est valide
    char chemin[100]; // Chemin du fichier de sauvegarde
    sprintf(chemin, "../assets/sauvegardes/%s.txt", id); // On ajoute l'id à la fin du chemin

    return access(chemin, F_OK) != -1; // On vérifie si le fichier existe
}

struct ModeleNiveau charger_sauvegarde(char *id, int code) { // Charge une sauvegarde
    char nouvel_id[50]; // Nouvel id de la sauvegarde
    if(code == -1) { // Si la sauvegarde n'existe pas
        system("cls");
        afficher_fichier("../assets/ASCII/charger_partie.txt"); // Afficher le logo
        wprintf(L"Veuillez entrer l'id de la sauvegarde : \n"); // Demander l'id de la sauvegarde
        scanf("%s", nouvel_id); // Récupérer l'id de la sauvegarde
        return charger_sauvegarde(nouvel_id, 0); // Charger la sauvegarde, code = 0 : on passe au else
    } else if(code == -2) { // Si la sauvegarde est invalide
        wprintf(L"Sauvegarde invalide, Veuillez reessayer : \n");
        scanf("%s", nouvel_id);
        return charger_sauvegarde(nouvel_id, 0); // Charger la sauvegarde, code = 0 : on passe au else
    } else { // Code = 0, on teste si la sauvegarde existe, si oui on la charge, sinon on recommence
        wprintf(L"Chargement de la sauvegarde \"%s\"\n", id);
        if(!sauvegarde_id_valide(id)) // Si la sauvegarde n'existe pas
            return charger_sauvegarde(nouvel_id, -2); // On recommence, code = -2 : on passe au if(code == -2)
        else { // Si la sauvegarde existe
            struct ModeleNiveau sauvegarde = modele_niveau(id, 1); // On charge la sauvegarde
            jeu(sauvegarde); // On lance le jeu
        }
    }
}


void sauvegarder_partie(struct ModeleNiveau modele, int temps_restant, char* chemin) { // Sauvegarde la partie
    FILE *fichier = fopen(chemin, "w"); // On ouvre le fichier en écriture
    fprintf(fichier, "%d\n", temps_restant); // On écrit le temps restant
    fprintf(fichier, "%d\n", modele.niveau); // On écrit le niveau
    fprintf(fichier, "%d\n", modele.vies_restantes); // On écrit les vies restantes
    fprintf(fichier, "%d\n", modele.score); // On écrit le score
    fprintf(fichier, "%d\n", modele.sous_case); // On écrit la sous-case
    fprintf(fichier, "%d", modele.nb_balles); // On écrit la sous-case

    for(int i = 0; i < modele.nb_balles; i++) {
        fprintf(fichier, "\n%d", modele.balles[i].x); // On écrit la position x de la balle
        fprintf(fichier, "\n%d", modele.balles[i].y); // On écrit la position y de la balle
        fprintf(fichier, "\n%d", modele.balles[i].direction); // On écrit la direction de la balle
    };
//    Ajout de la matrice
    for (int i = 0; i < modele.hauteur; i++) { // Pour chaque ligne
        fprintf(fichier, "\n"); // On saute une ligne
        for (int j = 0; j < modele.largeur; j++) { // Pour chaque colonne
            fprintf(fichier, "%d", modele.modele[i][j]); // On écrit la valeur de la case
        }
    }
    fclose(fichier); // On ferme le fichier
}

char *demande_sauvegarde_id() { // Demande l'id de la sauvegarde
    char *nom = malloc(50 * sizeof(char)); // On alloue de la mémoire pour le nom
    wprintf(L"Veuillez entrer le nom de la sauvegarde :"); // Demander le nom de la sauvegarde
    scanf("%s", nom); // Récupérer le nom de la sauvegarde

    char chemin[100]; // Chemin du fichier de sauvegarde
    sprintf(chemin, "../assets/sauvegardes/%s.txt", nom); // On ajoute le nom à la fin du chemin

    if(access(chemin, F_OK) != -1) { // Si le fichier existe
        wprintf(L"Une sauvegarde avec ce nom existe deja, voulez vous l'ecraser ? (o/n)\n"); // Demander si on veut l'écraser
        char reponse; // Réponse
        scanf("%c", &reponse); // Récupérer la réponse
        if(reponse == 'n') return demande_sauvegarde_id(); // Si la réponse est non, on recommence
    }

    return nom; // On retourne le nom
}