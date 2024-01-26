#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "menu.h"
#include "../tools/tools.h"
#include "../niveau/niveau.h"
#include "../partie/partie.h"
#include "../sauvegardes/sauvegardes.h"

void menu() { // Affiche le menu et lance les fonctions en fonction du choix de l'utilisateur
    afficher_menu(); // Affiche le menu
    int option = selectionne_option(); // Demande à l'utilisateur de choisir une option
    switch (option) { // Lance la fonction correspondante
        case 1:
            afficher_regles(); // Affiche les règles
            break;
        case 2:
            nouvelle_partie("1"); // Lance une nouvelle partie avec le niveau 1
            break;
        case 3:
            charger_sauvegarde("", -1); // Lance le processus de chargement d'une sauvegarde
            break;
        case 4:
            ;char id[10]; // On prépare la mémoire pour stocker l'id du niveau
            wprintf(L"Quel niveau voulez-vous charger ?\n"); // On demande à l'utilisateur de choisir un niveau
            scanf("%s", id); // On récupère l'id du niveau
            nouvelle_partie(id); // On lance une nouvelle partie avec l'id du niveau
            break;
        case 5:
            scores(); // Affiche les scores
            break;
        case 6:
            quitter(); // Quitte le jeu
            break;
        default:
            system("cls"); // Efface le terminal
            menu(); // Relance le menu
    }
};
void afficher_menu() { // Affiche le menu
    afficher_fichier("../assets/menu.txt"); // Affiche le fichier menu.txt
}
int selectionne_option() { // Demande à l'utilisateur de choisir une option
    int option; // On prépare la mémoire pour stocker l'option
    scanf("%d", &option); // On récupère l'option

    return option; // On retourne l'option
}

void afficher_regles() { // Affiche les règles
    system("cls"); // Efface le terminal
    afficher_fichier("../assets/ASCII/regles.txt"); // Affiche le fichier regles.txt
    menu(); // Relance le menu
}
void scores() {
    system("cls"); // Efface le terminal
    // wprintf: affiche du texte en unicode (L"..." pour les chaines de caractères unicode, %ls pour les chaines de caractères unicode dans un printf, notamment pour les symboles)
    wprintf(L"Malheureusement, les scores ne sont pas encore disponibles...\nNous ne savons pas a quoi sert cette commande !"); // Affiche un message
    usleep(250000); // Attend 1 seconde
    system("cls"); // Efface le terminal
    afficher_fichier("../assets/ASCII/logo.txt"); // Affiche le logo
    menu(); // Relance le menu
}
void quitter() { // Quitte le jeu
    afficher_fichier("../assets/ASCII/quitter.txt"); // Affiche le fichier quitter.txt
    sleep(3); // Attend 1 seconde
    exit(0); // Quitte le programme
}