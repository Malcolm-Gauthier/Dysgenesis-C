#include "Misc.h"

Curseur* CreerCurseur(Jeu* jeu) {

    if (jeu->curseur != NULL) {

        free(jeu->curseur);
    }

    Curseur* curseur = (Curseur*)malloc(sizeof(Curseur));

    InitSprite(&curseur->self, jeu);
}