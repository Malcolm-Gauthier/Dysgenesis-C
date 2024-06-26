#pragma once
#include "Definitions.h"

int CreerJoueur(Joueur* joueur);

void InitializerJoueur(Joueur* joueur);

SDL_bool ExistJoueur(Joueur* joueur);

int TirJoueur(Joueur* joueur);

void AnimationMort(Joueur* joueur);

void BougerJoueur(Joueur* joueur);

SDL_bool JoueurMort(Joueur* joueur);