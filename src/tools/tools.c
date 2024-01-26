#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#include "tools.h"

void afficher_fichier(char *path) { // Affiche un fichier
    FILE *fichier = fopen(path, "r"); // On ouvre le fichier en lecture

    if(fichier == NULL) { // Si le fichier n'existe pas
        perror("Impossible d'ouvrir le fichier");
    }

    char ligne[100]; // On initialise une variable pour la ligne
    while(fgets(ligne, sizeof ligne, fichier) != NULL) // Tant qu'on n'est pas à la fin du fichier
        wprintf(L"%s", ligne); // On affiche la ligne
    wprintf(L"\n"); // On saute une ligne
    fclose(fichier); // On ferme le fichier
}

void COULEUR(int couleurDuTexte,int couleurDeFond) { // Change la couleur du texte
    HANDLE H=GetStdHandle(STD_OUTPUT_HANDLE); // On récupère le handle de la console
    SetConsoleTextAttribute(H,couleurDeFond*16+couleurDuTexte); // On change la couleur du texte
}
