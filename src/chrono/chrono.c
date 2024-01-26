#include <time.h>
#include "chrono.h"

int nouveau_chrono() {
//    120s en plus du temps actuel
//    Retourne le timestamp de fin (timestamp: nombre de secondes écoulées depuis le 1er janvier 1970)
    return (int)time(NULL) + 120;
    // temps restant = timestamp de fin - timestamp actuel
}