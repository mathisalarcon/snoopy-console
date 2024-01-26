#include "../niveau/niveau.h"

#ifndef SNOOPY_DEPLACEMENTS_H
#define SNOOPY_DEPLACEMENTS_H

void deplacer(struct ModeleNiveau *modele, char direction, char *derniere_direction);
void casse_bloc(struct ModeleNiveau *modele, const char *direction);
void deplacer_balle(struct ModeleNiveau *modele, int i);
#endif //SNOOPY_DEPLACEMENTS_H
