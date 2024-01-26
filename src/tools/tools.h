#ifndef SNOOPY_TOOLS_H
#define SNOOPY_TOOLS_H

#define NOIR 0
#define BLEU_FONCE 1
#define VERT_FONCE 2
#define TURQUOISE 3
#define ROUGE_FONCE 4
#define VIOLET 5
#define VERT_CACA_DOIE 6
#define GRIS_CLAIR 7
#define GRIS_FONCE 8
#define BLEU_FLUO 9
#define VERT_FLUO 10
#define TURQUOISE_2 11
#define ROUGE_FLUO 12
#define VIOLET_2 13
#define JAUNE 14
#define BLANC 15

struct Coordonnees {
    int x;
    int y;
};

void afficher_fichier(char *path);
void COULEUR(int couleurDuTexte,int couleurDeFond);

#endif //SNOOPY_TOOLS_H
