#pragma once
#include "Definitions.h"

Ennemi* CreerEnnemi(Jeu* jeu, TypeEnnemi type, StatusEnnemi statut, Ennemi* parent);

SDL_bool ExistEnnemi(Ennemi* ennemi);

void ActualiserModeleEnnemi(Ennemi* ennemi);

SDL_bool CodeBoss(Ennemi* ennemi);

int CollisionProjectileEnnemi(Ennemi* ennemi, Projectile* projectile);

int CollisionJoueurEnnemi(Ennemi* ennemi, Joueur* joueur);

Vector2 TrouverCibleEnnemi(Ennemi* ennemi);

void BougerEnnemi(Ennemi* ennemi);

void TirEnnemi(Ennemi* ennemi);

void RenderEnnemi(Ennemi* ennemi);