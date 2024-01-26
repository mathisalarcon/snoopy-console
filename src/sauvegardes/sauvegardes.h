#include "../niveau/niveau.h"

#ifndef SNOOPY_SAUVEGARDES_H
#define SNOOPY_SAUVEGARDES_H

int sauvegarde_id_valide(char *id);
struct ModeleNiveau charger_sauvegarde(char *id, int code);
void sauvegarder_partie(struct ModeleNiveau modele, int temps_restant, char *chemin);
char *demande_sauvegarde_id();

#endif //SNOOPY_SAUVEGARDES_H
