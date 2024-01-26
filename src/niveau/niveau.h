#include "../tools/tools.h"

#ifndef SNOOPY_NIVEAU_H
#define SNOOPY_NIVEAU_H

struct Dimensions {
    int largeur;
    int hauteur;
};
struct Balle {
    int x;
    int y;
    int direction;
};
struct ModeleNiveau {
    char *id;
    int **modele;
    int hauteur;
    int largeur;
    int niveau;
    int temps_restant;
    int vies_restantes;
    int nb_oiseaux;
    int nb_teleporteurs;
    int nb_balles;
    char *message;
    int score;
    int sous_case;
    struct Coordonnees snoopy;
    struct Coordonnees *oiseaux;
    struct Coordonnees *teleporteurs;
    struct Balle *balles;
};

void afficher_niveau(struct ModeleNiveau modele, int temps_restant, char derniere_direction);
struct ModeleNiveau modele_niveau(char *id, int sauvegarde);
struct Dimensions dimensions_niveau(int niveau);

int unite_chrono_restant(int prct_restant, int hauteur, int largeur);
int nombre_oiseaux(struct ModeleNiveau modele);

#endif //SNOOPY_NIVEAU_H
