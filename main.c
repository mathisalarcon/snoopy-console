#include <stdio.h>
#include <fcntl.h>
#include <io.h>

#include "src/menu/menu.h"
#include "src/tools/tools.h"
#include "src/niveau/niveau.h"
#include "src/sauvegardes/sauvegardes.h"


int main() {
    // Console en UTF-8 (utilise wprintf au lieu de printf, et les librairies unicode "fcntl.h" et "io.h")
    _setmode(_fileno(stdout), _O_U16TEXT);
    // Couleur du texte en Turquoise
    COULEUR(TURQUOISE, NOIR);
    // Affichage du logo
    afficher_fichier("../assets/ASCII/logo.txt");

    // Affichage du menu
    menu();
}